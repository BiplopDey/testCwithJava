# Use the official maven/Java image to create a build artifact.
FROM maven:3.8.1-openjdk-11-slim AS build
WORKDIR /usr/src/app
COPY . .
RUN mvn package

# Use the official openjdk image for a lean production stage of our multi-stage build.
FROM openjdk:11-jre-slim
COPY --from=build /usr/src/app/target/spring-boot-jni-example-0.0.1-SNAPSHOT.jar /usr/app/spring-boot-jni-example.jar
COPY --from=build /usr/src/app/libhello.so /usr/lib/libhello.so
EXPOSE 8080
ENTRYPOINT ["java", "-Djava.library.path=/usr/lib", "-jar", "/usr/app/spring-boot-jni-example.jar"]
