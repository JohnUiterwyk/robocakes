//
//  NetTest1.c
//  SocketTest
//
//  Created by John Uiterwyk on 9/8/13.
//  Copyright (c) 2013 John Uiterwyk. All rights reserved.
//
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


int main()
{
    char hostname[80];
    int result;
    printf("NetTest1\n");
    printf("Getting host name...\n");
    result = gethostname(hostname,sizeof(hostname));
    if(result == -1)
    {
        printf("Error getting host name: %s.\n",strerror(errno));
    }else
    {
        printf("This computers host name is %s.\n",hostname);
    }
    return 0;
}
