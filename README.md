# Projet_FAR

This project is an instant messaging app, for Polytech Montpellier FAR.

Made by Eugénio BAYE and Hugo BRUN.

## Getting started

### Prerequisites

Work on a linux environment.

Have gcc compiler installed.

Have git installed if you plan on cloning the repo.

### Installation

Clone the repo with git :
```
git clone https://github.com/Eugenio-BAYE/Projet_FAR
```
Move to "Setup" part.

Or download the app from https://github.com/Eugenio-BAYE/Projet_FAR,
Extract the downloaded file.

### Setup

Install the app on 3 computers

Use the Makefile to compile the files :
```
make
```

## Usage

### Launching the app

Open a terminal window on the server computer and get the ip:
```
ifconfig
```
Then, lauch the server file with the desired port (between 2000 and 8000)
```
./server <port>
```

Open a terminal window on another computer and launch the client file :
```
./client <server_ip> <port> 1
```

Open a terminal window on another computer and launch the client file :
```
./client <server_ip> <port> 2
```

### Using the app

When it's the turn of a client to send a message, type the message in the terminal window and press enter. The other client should receive the message and then can type its message.
