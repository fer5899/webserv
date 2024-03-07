#!/usr/bin/env python3

import os

print("Content-Type: text/html")
print()  # Imprimir una línea en blanco para separar las cabeceras del cuerpo del mensaje

print("<html><head><title>Prueba de CGI</title></head><body>")
print("<h1>¡Hola desde CGI!</h1>")
print("<p>Variables de entorno:</p>")
print("<ul>")
for key, value in os.environ.items():
    print(f"<li>{key} = {value}</li>")
print("</ul>")
print("</body></html>")
