/*
 * sepa_secure.h
 * 
 * Copyright 2017 Francesco Antoniazzi <francesco.antoniazzi@unibo.it>
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * 
 */

#ifndef SEPA_SECURE
#define SEPA_SECURE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "sepa_utilities.h"

#define _init_sClient()		{.client_id=NULL,.client_secret=NULL}

typedef struct secure_client {
	char *client_id;
	char *client_secret;
} sClient;

sClient registerClient(const char * identity,const char * registrationAddress);
void fprintfSecureClientData(FILE * outstream,sClient client_data);

#endif
