package com.example.demo;

import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
public class HelloController {

    @GetMapping("/hello")
    public String hello() {
        new HelloJNI().sayHello();
        return "Check console for hello from C!";
    }

    @GetMapping("/sum")
    public String sum() {
        char[][] matrix = {
                {0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0},
                {0, 0, 2, 2, 0},
                {0, 0, 1, 1, 0},
                {0, 0, 1, 1, 0}
        };
        int sum = new HelloJNI().sumMatrix(matrix);
        return "Sum of matrix elements: " + sum;
    }
}