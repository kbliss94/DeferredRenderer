###Overview

This is the source code repository for the video series *DirectX Essentials LiveLessons* by Paul Varcholik.

[![DirectX Essentials LiveLessons](http://www.varcholik.org/DirectXEssentialsLiveLessons/Cover.jpg)](http://my.safaribooksonline.com/video/illustration-and-graphics/9780134030036)

You'll find a forum for discussions and questions about the videos at [http://www.varcholik.org/](http://www.varcholik.org/).

###Dependencies

All lessons require Visual Studio 2013 (any sku, including [Express 2013 for Windows Desktop](http://www.visualstudio.com/en-us/products/visual-studio-express-vs.aspx)).

###Update 7/13/2016 - Visual Studio 2015 branch

A VS2015 branch is now available for this repository. This branch contains a signficant overhaul of the source code
for these lessons and touches almost every file in the repository. Below is a summary of the changes:

* Visual Studio 2015 projects
* Pre-compiled headers
* Project references to the Library project
* The Library project has been reorganized to support multiple platforms (e.g. UWP)* 
* NuGet projects for DirectXTK and assimp
* Increased warning level (and now treating warnings as errors)
* More C++11 usage
* Replaced most raw pointers with smart pointers
* Additional refactoring for [C++ Core Guidelines](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md) 
* Bug fixes