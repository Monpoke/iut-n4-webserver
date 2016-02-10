/* 
 * File:   client.h
 * Author: bourgeop
 *
 * Created on 3 février 2016, 15:25
 */

#ifndef CLIENT_H
#define	CLIENT_H

void clientLoop(int ID, int socket_client);
void processHeaderLine(int socket,int nb, char buffer[]);
void callError(int socket,int nb);
void removeSpecialCar(char *buffer);
void showWelcome(int socket_client);


#endif	/* CLIENT_H */

