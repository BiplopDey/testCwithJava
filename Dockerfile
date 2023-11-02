# Use a specific version for reproducibility
FROM maven:3.8.1-openjdk-11-slim AS build
WORKDIR /usr/src/app

# Only copy the pom.xml initially. This allows Docker to cache the image layers up to the `mvn dependency:go-offline` command.
# As long as your dependencies don't change, Docker will use the cached layers, and you won't have to download your dependencies every time you build your image.
COPY pom.xml .
RUN mvn dependency:go-offline

# Now copy the rest of the source, this will invalidate the cache only if there are changes in the source code.
COPY . .

# Compile C code to shared library. No change needed here.
RUN apt-get update && \
    apt-get install -y gcc && \
    gcc -shared -fPIC -o libhello.so src/main/c/hello.c -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux && \
    # Clean up in the same layer to keep the image size down.
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Build the Java application
# Skipping the tests with -DskipTests=true can significantly speed up the build if tests are not needed at this stage.
RUN mvn package -DskipTests

# Use the official openjdk image for a lean production stage of our multi-stage build.
FROM openjdk:11-jre-slim
# No need to set a WORKDIR if we are just going to run the app, but it can be useful for debugging.
WORKDIR /usr/app
COPY --from=build /usr/src/app/target/demo-0.0.1-SNAPSHOT.jar ./demo.jar
COPY --from=build /usr/src/app/libhello.so /usr/lib/

# EXPOSE instruction is for documentation purposes only, it does not actually publish the port.
EXPOSE 8080

# Use exec form of ENTRYPOINT to make sure Java process receives SIGTERM signals.
ENTRYPOINT ["java", "-Djava.library.path=/usr/lib", "-jar", "demo.jar"]
