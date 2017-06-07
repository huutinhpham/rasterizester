Rasterizester
===================================

# Overview

This project aims to render 2D images. First every pixels between two given points were found by using simple 2D math. Then I scanned in both x, and y directions to render those pixels to create a line. With three given points, barycentric formula was used to determine if a pixel is inside the triangle. Supersampling is used to anti-alias the triangles, while simple matrix mulplications were used to transform images.


### Follow the link for full explanation of the mathods made by me

[CLICK ME FOR DOCUMENTATIONS](https://huutinhpham.github.io/projects/rasterizester/rasterizester.html)

##### Credit

All credit for the project reserved to UC Berkeley CS 184 with professor Ren Nguyen.
