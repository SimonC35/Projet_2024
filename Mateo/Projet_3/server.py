from http.server import HTTPServer, SimpleHTTPRequestHandler
import sys

class CORSRequestHandler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header('Access-Control-Allow-Origin', '*')
        super().end_headers()

port = 8080
httpd = HTTPServer(('localhost', port), CORSRequestHandler)
print(f"Serveur démarré sur http://localhost:{port}")
httpd.serve_forever()
