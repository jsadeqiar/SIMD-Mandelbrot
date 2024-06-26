<a name="readme-top"></a>

<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in bracketts [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->
<div align="center">

[![LinkedIn][linkedin-shield]][linkedin-url]
</div>


<!-- PROJECT LOGO -->
<br />
<div align="center">
  <a href="https://github.com/jsadeqiar/SIMD-Mandelbrot">
    <img src="https://raw.githubusercontent.com/jsadeqiar/SIMD-Mandelbrot/main/ico/mand.png?token=GHSAT0AAAAAACJ4THU7RVHNBMNYKZCKRNFQZQDTGSA" alt="Logo" width="80" height="80">
  </a>

<h3 align="center">SIMD-Mandelbrot</h3>

  <p align="center">
    A Mandelbrot set explorer with Multi-threading and SIMD support!
    <br />

  </p>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#contact">Contact</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

[![Product Name Screen Shot][product-screenshot]](https://github.com/jsadeqiar/SIMD-Mandelbrot)

SIMD-Mandelbrot is an interactive viewer of the Mandelbrot set on the complex plane. It allows for the panning and zooming across the plane, setting the iteration count to judge how deep the set is defined as well as different methods of computation, which is meant to be the showcase of the project.

Within this project you're able to view and analyze the performance of how the set is calculated with:
* Single-threading
* Multi-threading
* Multi-threading w/ SIMD instructions (AVX2 instruction set)

A benchmarking option is also available which will show the various frametime comparisons between the 3 calculation methods.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



### Built With
<div align="center">

 ![C++][C++]
 [![DearImgui][dearimgui]][dearimgui-url]
 [![SDL3][SDL3]][SDL3-url]
 [![xsimd][xsimd]][xsimd-url]
 [![CMake][CMake]][CMake-url]
</div>

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

* A CPU which supports the AVX2 instruction set.

### Installation

1. Clone the repository and submodules:
   ```
   git clone --recurse-submodules https://github.com/jsadeqiar/SIMD-Mandelbrot.git
   ```
2. Create a build directory:
   ```
   mkdir build && cd build
   ```
3. Run CMake:
   ```
   cmake ..
   cmake --build . --config Release
   cd Release
   ```
4. Run the program:
   ```
   ./SIMD-MANDELBROT.exe
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage
Aside from using the UI to press the desired input, the following key inputs are supported:

<!-- TABLE_GENERATE_START -->

| Key Input  | Usage |
| ------------- | ------------- |
| W  | Pan Up  |
| A  | Pan Left  |
| S  | Pan Down  |
| S  | Pan Right  |
| F  | Zoom In  |
| G  | Zoom Out  |
| R  | Reset Position  |

<!-- TABLE_GENERATE_END -->

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact

Jawid Sadeqiar  - jsadeqiar [at] outlook [dot] com

LinkedIn - [https://www.linkedin.com/in/jawid-sadeqiar/](https://www.linkedin.com/in/jawid-sadeqiar/)

Project Link: - [https://github.com/jsadeqiar/SIMD-Mandelbrot](https://github.com/jsadeqiar/SIMD-Mandelbrot)

<p align="right">(<a href="#readme-top">back to top</a>)</p>




<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->

[product-screenshot]: https://raw.githubusercontent.com/jsadeqiar/SIMD-Mandelbrot/main/ico/program_screenshot.png?token=GHSAT0AAAAAACJ4THU6K75UBIBFJR73AEYKZQDTHKQ
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://www.linkedin.com/in/jawid-sadeqiar/
[C++]: https://img.shields.io/badge/c%2B%2B%20-%20grey?style=for-the-badge
[dearimgui]: https://img.shields.io/badge/dear%20imgui%20-%20lightblue?style=for-the-badge
[dearimgui-url]: https://github.com/ocornut/imgui
[SDL3]: https://img.shields.io/badge/SDL3%20-%20blue?style=for-the-badge
[SDL3-url]: https://github.com/libsdl-org/SDL
[xsimd]: https://img.shields.io/badge/xsimd-%20lightgreen?style=for-the-badge
[xsimd-url]: https://github.com/xtensor-stack/xsimd
[CMake]: https://img.shields.io/badge/CMake%20-%20g?style=for-the-badge&color=%23064F8C
[CMake-url]: https://cmake.org/