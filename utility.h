#ifndef __UTILITY_H__
#define __UTILITY_H__


char *query_by_get(){
	return getenv("QUERY_STRING") ;
}


char *query_by_post(){
	static int capacity = 0 ;
	static char *buffer = NULL ;

	// get length of query
	int query_len = atoi( getenv("CONTENT_LENGTH") ) ;
	if( query_len == 0 )
		return NULL ;

	// enlarge capacity if necessary
	if( capacity < query_len ){
		if( buffer )
			free( buffer ) ;

		capacity = query_len *2 ;
		buffer = (char*) malloc( capacity ) ;
	}

	// reset buffer
	memset( buffer, 0, query_len ) ;

	// read query
	char scanf_format[8] ;
	sprintf( scanf_format, "%%%dc", query_len ) ;
	scanf( scanf_format, buffer ) ;

	return buffer ;
}


char *parse_query( char *seek, char **key, char **value, char split ){
	*key = seek ;
	for( ; *seek && *seek!='=' ; seek++ ) ;
	if( *seek ){
		*seek = '\0' ;
		seek ++ ;
	}

	*value = seek ;
	for( ; *seek && *seek!=split ; seek++ ) ;
	if( *seek ){
		*seek = '\0' ;
		seek ++ ;
	}

	return seek ;
}


void gen_token( char *token, int len_token, const char *user, const char *pass ){
	int i ;
	for( i=0 ; i<len_token ; ){
		token[i++] = *(user++) ;
		if( i<len_token )
			token[i++] = *(pass++) ;
	}
	token[len_token-1] = '\0' ;
}

#endif
