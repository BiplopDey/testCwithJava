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
        int[][] matrix = {
                {1, 2, 3},
                {4, 5, 6},
                {7, 8, 9}
        };
        int sum = new HelloJNI().sumMatrix(matrix);
        return "Sum of matrix elements: " + sum;
    }
}