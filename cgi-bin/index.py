# -*- coding: utf-8 -*-

import cgi

# Configurar el encabezado Content-Type
print("Content-Type: text/html\n")

# Imprimir el contenido HTML
print("<html>")
print("<head><title>Formulario CGI en Python</title></head>")
print("<body>")
print("<h1>Formulario CGI en Python</h1>")
print("<form method='post' action='script.py'>")
print("<label for='nombre'>Nombre:</label>")
print("<input type='text' id='nombre' name='nombre'><br>")
print("<input type='submit' value='Enviar'>")
print("</form>")

# Procesar datos de entrada si están presentes
form = cgi.FieldStorage()
if "nombre" in form:
    nombre = form["nombre"].value
    print("<p>¡Hola, {}!</p>".format(nombre))

print("</body>")
print("</html>")