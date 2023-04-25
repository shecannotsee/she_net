import socket

def tcp_client(host, port, message):
    # 创建一个TCP套接字
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # 连接服务器
    client_socket.connect((host, port))

    # 发送消息
    client_socket.sendall(message.encode())

    # 接收响应并返回
    response = client_socket.recv(1024)

    # 关闭连接
    client_socket.close()

    return response.decode()

if __name__ == '__main__':
    host = '127.0.0.1'
    port = 9981
    message = 'Hello, server!'
    response = tcp_client(host, port, message)
    print(f'Received {len(response)} bytes: {response}')
