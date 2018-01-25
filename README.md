Rasterizester
===================================

# Overview

This project aims to render 2D images. First every pixels between two given points were found by using simple 2D math. Then I scanned in both x, and y directions to render those pixels to create a line. With three given points, barycentric formula was used to determine if a pixel is inside the triangle. Supersampling is used to anti-alias the triangles, while simple matrix mulplications were used to transform images.


### Follow the link for full explanation of the methods made by me

[CLICK ME FOR DOCUMENTATIONS](http://54.241.219.178/projects/Rasterizester)

##### Credit

All credits for the project are reserved to UC Berkeley CS 184 with professor Ren Nguyen.
