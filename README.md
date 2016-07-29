# TCP_Socket
Using Qt console mode to implement socket.

# Notice 
1. The server name and port now are static , which stick to localhost and 51617. You can change it in /TestClient.
2. Support only console mode. (7/23)

# Usage 
  1. Windows Version : 
    - First , copy the hold /bin directory from Qt (the source in your computer) , and drag your /Debug ( or /Release ) within.
    - Then open the .exe in /TestServer for server side , and /TestClient for client side.
    - After open the client side , you can see the info from each side , and receive successfully.
  - For download :
    - In /TestServer/available_download : the file that user can view from client , which user can using download command on those file.
    - In /TestClient/DownloadTest : the file that user download from server , store here.
  2. Linux Version :
    - Only need to using Qt creator to open this project (TestServer or TestClient) , then clear the hold project and rebuild it.

# Functional Support
1. help : list out the command that server support
2. quit : disconnect by user
3. ls : list out all the file from server's current directory
4. download_< filename > : download file from server , which user can assign the specific name in filename.

# Environment
  - (7/23) : Qt 5.7.0 Windows version (MinGW-32bits)
  - (7/29) : Qt 5.7.0 Linux version (GCC version 5.4.0)

# Author
  - Kevin Chu (kevinbird61@gmail.com)

