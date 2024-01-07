/*
Criado por Marcelo Maurin Martins
Data: 30/01/2014
*/
 
#include<stdio.h>
#include<string.h>    //strlen
#ifdef _WIN32

#endif

#ifdef _WIN64

#endif

#ifdef _LINUX
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include <espeak/speak_lib.h>
#endif



#define PORTSRV 8096

#ifdef _LINUX
int socket_desc, client_sock, c, read_size;
struct sockaddr_in server, client;
char client_message[2000];

#endif

#ifdef _LINUX
espeak_AUDIO_OUTPUT output;
#endif

#ifdef _LINUX
void Ler(char * frase) {
	int speakErr =0;
	if((speakErr=espeak_Synth(frase, strlen(frase), 0,0,0,espeakCHARS_AUTO,NULL,NULL))!= EE_OK){
		printf("Error on synth creation\n");
	}
	espeak_Synchronize();
}
#endif

#ifdef _WIN32
int Start_Voice() 
{
    return 0;
}
#endif

#ifdef _WIN64 
int Start_Voice()
{
    return 0;
}
#endif


#ifdef _LINUX
int Start_Voice()
{
	output = AUDIO_OUTPUT_PLAYBACK;
	int Buflength = 500;
	int Options=0;
	char *path=NULL;
	if(espeak_Initialize(output, Buflength, path, Options)<0){
		puts("could not initialize espeak\n");
		return -1;
	} else {
		puts("Ok\n");
	}
	espeak_SetVoiceByName("pt");
	espeak_VOICE *voice_spec=espeak_GetCurrentVoice();
	voice_spec->gender =2; //0=none 1=male, 2=female
	voice_spec->age=0; //0=not specified, or age in years
	espeak_SetVoiceByProperties(voice_spec);
	//espeak_SetSynthCallback(SynthCallback);
	espeak_Synchronize( );
	//t_espeak_callback *SynthCallback;
	return 0;	
}
#endif

#ifdef _WIN32
int controlesocket_windows32()
{
  return 0;
}
#endif

#ifdef _WIN64
int controlesocket_windows64()
{
  return 0;
}
#endif

#ifdef _LINUX
int controlesocket_linux()
{
    //Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");


    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORTSRV);

    //Bind
    if (bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
    int flgAtivo = 1;
    while (flgAtivo == 1)
    {
        //Listen
        listen(socket_desc, 3);

        //Accept and incoming connection
        puts("Waiting for incoming connections...");
        c = sizeof(struct sockaddr_in);
        //while(flgAtivo==1)

        //accept connection from an incoming client
        client_sock = accept(socket_desc, (struct sockaddr*)&client, (socklen_t*)&c);
        if (client_sock < 0)
        {
            perror("accept failed");
            return 1;
        }
        puts("Connection accepted");
        memset(client_message, '\0', sizeof(client_message));
        //Receive a message from client
        while ((read_size = recv(client_sock, client_message, 2000, 0)) > 0)
        {
            printf("%s\n", client_message);
            //Send the message back to client
            //write(client_sock , client_message , strlen(client_message));
            //printf(client_message);
            Ler(client_message);
            memset(client_message, '\0', sizeof(client_message));
        }

        if (read_size == 0)
        {
            puts("Client disconnected");
            fflush(stdout);
        }
        else if (read_size == -1)
        {
            perror("recv failed");
        }
    }
}
#endif

int main(int argc , char *argv[])
{

    Start_Voice();
     
    #ifdef _WIN32
    controlesocket_windows32();
    #endif

    #ifdef _WIN64
    controlesocket_windows64();
    #endif

    #ifdef _LINUX
	controlesocket_linux();
    #endif

    return 0;
}

