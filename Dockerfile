# Start with a base image that has Maven and JDK
FROM maven:3.8.1-openjdk-17-slim AS build
WORKDIR /usr/src/app
COPY . .

# Install gcc and necessary tools
RUN apt-get update && \
    apt-get install -y gcc && \
    rm -rf /var/lib/apt/lists/*

# Compile C code to shared library
RUN gcc -shared -fPIC -o libhello.so hello.c -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux

# Build the Java application
RUN mvn package

# Use the official openjdk image for a lean production stage of our multi-stage build.
FROM openjdk:11-jre-slim
COPY --from=build /usr/src/app/target/demo-0.0.1-SNAPSHOT.jar /usr/app/demo.jar
COPY --from=build /usr/src/app/libhello.so /usr/lib/libhello.so
EXPOSE 8080
ENTRYPOINT ["java", "-Djava.library.path=/usr/lib", "-jar", "/usr/app/demo.jar"]
