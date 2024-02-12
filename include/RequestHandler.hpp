#pragma once

#include <iostream>
#include <map>
#include <vector>
#include "utils.hpp"

#define MAX_REQUEST_SIZE 1000000
//COLORS
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN "\033[1;36m"
#define RESET "\033[0m"

class RequestHandler
{
	private:
		//Info to send to the response
		std::string	_path;
		std::string	_method;
		std::string	_version;
		std::map<std::string, std::string> _headers;
		std::string	_body;
		int			_errorCode;


		//Self info
		std::string	_buffer;
		size_t		_size;
		double		_bodySize;
		int			_state;

		bool 		parseFirstLine(std::string& line);
		bool		parseHeaders(std::string& line);
		bool		parseBodyRequisites();
		bool		parseBody(std::string& line);
	public:
		RequestHandler();
		~RequestHandler();
		RequestHandler& operator=(const RequestHandler& other);
		RequestHandler(const RequestHandler& other);
		std::map<std::string, std::string>	getHeaders() const;
		std::string	getMethod() const;
		std::string	getURL() const;
		std::string	getHTTPVersion() const; // Si solo vamos a trabajar con 1.1 esto va fuera
		std::string	getBody() const;
		bool 		parseRequest(std::string& request); // Devuelve true si la request ha fallado en algo o esta completa
		int			getErrorCode() const;


		void		setErrorCode(int errorCode);
};


// 400 Bad Request (Solicitud incorrecta): Se devuelve cuando la sintaxis de la solicitud es incorrecta o no se puede entender por parte del servidor.
// 401 Unauthorized (No autorizado): Indica que el cliente debe autenticarse para obtener acceso al recurso solicitado, pero no ha proporcionado credenciales válidas o las credenciales son insuficientes.
// 403 Forbidden (Prohibido): El servidor comprende la solicitud del cliente, pero se niega a cumplirla. Esto suele ser debido a la falta de permisos de acceso al recurso solicitado.
// 404 Not Found (No encontrado): El servidor no puede encontrar el recurso solicitado. Este código de respuesta probablemente es el más famoso debido a su frecuencia en la web.
// 405 Method Not Allowed (Método no permitido): El método de solicitud es conocido por el servidor, pero ha sido deshabilitado y no puede ser utilizado.
// 408 Request Timeout (Tiempo de espera de la solicitud): El servidor ha esperado demasiado tiempo para la solicitud del cliente.
// 413 Payload Too Large (Carga útil demasiado grande): Se produce cuando el tamaño de la solicitud del cliente excede el límite establecido por el servidor.
// 415 Unsupported Media Type (Tipo de medio no soportado): Se devuelve cuando el servidor no puede manejar el tipo de medio de la solicitud (por ejemplo, un tipo de contenido en el cuerpo de la solicitud que no puede ser procesado).
// 500 Internal Server Error (Error interno del servidor): Es un código comúnmente emitido por aplicaciones empotradas en servidores web, como Apache o Nginx, y dedicado a la identificación de un error en el servidor que no puede ser más específico.
// 501 Not Implemented (No implementado): El servidor no soporta la funcionalidad necesaria para completar la solicitud.
// 503 Service Unavailable (Servicio no disponible): El servidor no puede responder a la solicitud del navegador porque está congestionado o está realizando tareas de mantenimiento.
// 504 Gateway Timeout (Tiempo de espera de la puerta de enlace): Esta respuesta de error es dada cuando el servidor está actuando como una puerta de enlace y no puede obtener una respuesta en el tiempo establecido.
// 505 HTTP Version Not Supported (Versión de HTTP no soportada): El servidor no soporta la versión del protocolo HTTP utilizada en la solicitud.
// 511 Network Authentication Required (Requiere autenticación de red): El cliente necesita autenticarse para obtener acceso a la red.

// Yo me deberia ocupar de 400, 405, 413, 415 y 505 y cortar el contacto con el cliente directamente si pasa alguno de ellos
// 401, 408, 503, 504, 511 Server
// 403, 404, 501 Response 
// 500? Configuración incorrecta del servidor web


// struct timeval timeout;
// timeout.tv_sec = 5; // 5 segundos
// timeout.tv_usec = 0;
// int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout); if (result = -1)