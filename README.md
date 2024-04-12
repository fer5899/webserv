<h1 align=center>Webserv CPP</h1>

> A lightweight web server implemented in C++.

This repository contains the source code for a web server written in C++. It aims to provide a simple and efficient solution for serving web content.

<h2 align=center>
  <a href="#about">About</a>
  <span> · </span>
  <a href="#installation">Installation</a>
  <span> · </span>
  <a href="#usage">Usage</a>
  <span> · </span>
  <a href="#features">Features</a>
  <span> · </span>
  <a href="#contributing">Contributing</a>
</h2>

## About

Webserv CPP is a lightweight and efficient web server implementation designed to handle HTTP requests and serve web content. It provides a customizable and extensible architecture for building web applications.

## Installation

Before you begin, ensure you have the following prerequisites installed:

- [C++ Compiler](#): Make sure you have a C++ compiler installed on your system.

- [Make](#): Make is a cross-platform build system. Make sure to have Make installed to build the project.

- [Git](#): If your project involves cloning a Git repository, ensure that Git is installed on your machine.

## Usage

To install Webserv CPP, follow these steps:

1. Clone the repository:

```
git clone https://github.com/fer5899/webserv.git
cd webserv
```

2. Build the server:
```
make
./webserv confs/default.conf
```

3. Explore:
- Go to localhost:4242
- Try curl
- Try siege
- Try things

Examples of curl/siege:
```
curl -X GET http://localhost:4242
```

```
siege -c 20 -r 20 http://localhost:4242  
```

## Features
- HTTP/1.1 Protocol Support: Webserv CPP supports the HTTP/1.1 protocol for handling HTTP requests.
- Static File Serving: Serve static files such as HTML, CSS, and JavaScript.
- Dynamic Content Generation: Generate dynamic content using server-side scripting languages. (Python and Shell script)
- Customizable Configuration: Configure server settings such as port number and document root.

## Contributing
Contributions are welcome! Feel free to submit pull requests or open issues for bug fixes, feature requests, or general improvements.