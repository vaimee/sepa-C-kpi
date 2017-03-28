/*
 * sepa_producer.c
 * 
 * Copyright 2017 Francesco Antoniazzi <francesco.antoniazzi@unibo.it>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include "sepa_producer.h"


long kpProduce(const char * update_string,const char * http_server) {
	CURL *curl;
	CURLcode result;
	struct curl_slist *list = NULL;
	long response_code;
	FILE *nullFile;
	
	result = curl_global_init(CURL_GLOBAL_ALL);
	if (result) {
		fprintf(stderr,"curl_global_init() failed.\n");
		return KPI_PRODUCE_FAIL;
	}
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, http_server);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, update_string);
		
		list = curl_slist_append(list, "\"Content-Type: application/sparql-update\"");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		
		nullFile = fopen("/dev/null","w");
		if (nullFile==NULL)	{
			fprintf(stderr,"Error opening /dev/null.");
			response_code = KPI_PRODUCE_FAIL;
		}
		else {
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, nullFile);
			result = curl_easy_perform(curl);
			if (result!=CURLE_OK) {
				fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(result));
				response_code = KPI_PRODUCE_FAIL;
			}
			else {
				curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&response_code);
			}
			fclose(nullFile);
		}
		curl_easy_cleanup(curl);
	}
	else {
		fprintf(stderr, "curl_easy_init() failed.\n");
		response_code = KPI_PRODUCE_FAIL;
	}
	curl_global_cleanup();
	return response_code;
}