# -*- coding: utf-8 -*-

import cgi

def print_html_header():
    print("Content-Type: text/html;charset=utf-8")
    print()
    print("<!DOCTYPE html>")
    print("<html>")
    print("<head>")
    print("<title>Python CGI Form</title>")
    print("<style>")
    print("body { font-family: Arial, sans-serif;}")
    print("form { margin-top: 20px;}")
    print("label { display: block; margin-bottom: 5px;}")
    print("input[type='text'], input[type='number'] { width: 200px; padding: 5px;}")
    print("input[type='submit'] { padding: 8px 20px; background-color: #4CAF50; color: white; border: none; cursor: pointer;}")
    print("</style>")
    print("</head>")
    print("<body>")
    print("<h1>Python CGI Form</h1>")
    print("<form method='post' action='script.py'>")
    print("<label for='fname'>First Name:</label>")
    print("<input type='text' id='fname' name='fname' required><br>")
    print("<label for='lname'>Last Name:</label>")
    print("<input type='text' id='lname' name='lname' required><br>")
    print("<label for='age'>Age:</label>")
    print("<input type='number' id='age' name='age' required><br>")
    print("<input type='submit' value='Submit'>")
    print("</form>")

def print_html_footer():
    print("</body>")
    print("</html>")

print_html_header()

form = cgi.FieldStorage()
if "fname" in form and "lname" in form and "age" in form:
    fname = form["fname"].value
    lname = form["lname"].value
    age = form["age"].value
    print("<p>Hello, {} {}! You are {} years old.</p>".format(fname, lname, age))

print_html_footer()
