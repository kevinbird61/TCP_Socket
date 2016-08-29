# TCP_Socket
Using Qt console mode to implement socket.

# Notice 
 - The server name and port now are static , which stick to localhost and 51617. You can change it in /TestClient.
 - Support only console mode. (7/23)
 
###### Bugs Report Record :
   - (8/18): Run in different server , change from the original test environment - windows (localhost with localhost) , to "[server - ubuntu] [client - windows 10]" . The result in original test environment with download and upload functions are perfect ; But run the new test environment has occurred the bugs on download and upload , which can't recognize the delivering packages. [need to fix]  [ P.S : Linux to Linux test environment work perfectly too ] 
   - (8/21): Run in different server (Both in Linux) , but the consequences are the same. Need to check with the QByteArray and QTcoSocket's transport function - write().
   - *`Fix Bugs`* (8/29): I narrow the size of each tcp sending packages , from 16384 bytes to 1024 bytes. And then upload and download function is complete. 

# Usage 
  1. Windows Version : 
    - First , copy the hold /bin directory from Qt (the source in your computer) , and drag your /Debug ( or /Release ) within.
    - Then open the .exe in /TestServer for server side , and /TestClient for client side.
    - After open the client side , you can see the info from each side , and receive successfully.
  2. Linux Version :
    - Only need to using Qt creator to open this project (TestServer or TestClient) , then clear the hold project and rebuild it.
  
  ###### For download & upload :
    - In /TestServer/available_download : the file that user can view from client , which user can using download command on those file.
    - In /TestClient/DownloadTest : the file that user download from server , store here.
    - In /TestServer/upload : store the file upload from the client side.
    - In /TestClient/upload : store the file that owned by the client. And they can be upload to server side with corresponding command.

# Functional Support
1. *help* : list out the command that server support
2. *quit* : disconnect by user
3. *ls* : list out all the file from server's current directory
4. *download_< filename >* : download file from server , which user can assign the specific name in filename.
5. *upload_tag1_tag2* : upload file to server , which **tag1** is denoted as file/dir , and **tag2** is denoted as the filename.

# Environment
  - (7/23) : Qt 5.7.0 Windows version (MinGW-32bits)
  - (7/29) : Qt 5.7.0 Linux version (GCC version 5.4.0)

# Author
  - Kevin Chu (kevinbird61@gmail.com)

