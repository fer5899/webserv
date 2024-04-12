import os

def print_html_header():
    print("<!DOCTYPE html>")
    print("<html>")
    print("<head>")
    print("<title>Python CGI Script</title>")
    print("</head>")
    print("<body>")
    print("<h1>Form Data</h1>")

def print_html_footer():
    print("</body>")
    print("</html>")

print_html_header()

variable = "BODY"
value = os.getenv(variable)

if value is not None:
    values_dict = {}
    pairs = value.split('&')
    for pair in pairs:
        key, val = pair.split('=')
        values_dict[key] = val
    print("<p>First Name: {}</p>".format(values_dict.get('fname', 'N/A')))
    print("<p>Last Name: {}</p>".format(values_dict.get('lname', 'N/A')))
    print("<p>Age: {}</p>".format(values_dict.get('age', 'N/A')))
else:
    print("<p>The environment variable {} is not defined.</p>".format(variable))

print_html_footer()
