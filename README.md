# TCP_Socket
Using Qt console mode to implement socket.

# Notice 
1. The server name and port now are static , which stick to localhost and 51617. You can change it in /TestClient.
2. Support only console mode. (7/23)

# Usage 
  - Windows Version : 
    - First , copy the hold /bin directory from Qt (the source in your computer) , and drag your /Debug ( or /Release ) within.
    - Then open the .exe in /TestServer for server side , and /TestClient for client side.
    - After open the client side , you can see the info from each side , and receive successfully.

# Functional Support
1. help : list out the command that server support
2. quit : disconnect by user
3. ls : list out all the file from server's current directory
4. download_<filename> : download file from server , which user can assign the specific name in filename.

# Environment
  - (7/23) : Qt 5.7.0 Windows version (MinGW-32bits)

# Author
  - Kevin Chu (kevinbird61@gmail.com)

