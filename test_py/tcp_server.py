import socket

def tcp_server(host, port):
    # 创建一个TCP套接字
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # 绑定到指定地址和端口
    server_socket.bind((host, port))

    # 监听连接
    server_socket.listen()

    print(f'Server listening on {host}:{port}...')

    while True:
        # 等待客户端连接
        client_socket, client_address = server_socket.accept()

        print(f'Client connected from {client_address[0]}:{client_address[1]}')

        # 接收消息
        message = client_socket.recv(1024)

        # 打印消息到控制台
        print(f'Received {len(message)} bytes: {message.decode()}')

        # 发送响应
        response = 'Hello, client!'
        client_socket.sendall(response.encode())

        # 关闭连接
        client_socket.close()

if __name__ == '__main__':
    host = '127.0.0.1'
    port = 9981
    tcp_server(host, port)
