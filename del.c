#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simple_db.h"
#include "utility.h"


void print_query_detail( const char *query ){
	// print query
	printf( "%s<br>\"%s\"<br><br>\n", "QUERY_STRING(POST)", query?query:"(NULL)" ) ;

	// print query detail
	int query_len = atoi( getenv("CONTENT_LENGTH") ) ;
	printf( "query_len: %d<br>", query_len ) ;

	int i ;
	for( i=0 ; i<query_len ; i++ ){
		if( query[i] == '\n' || query[i] == '=' )
			printf("<b>") ;

		printf( "query[%d]: ", i ) ;
		if( query[i] == '\n' )
			printf( "'%s'", "\\n" ) ;
		else
			printf( "'%c'", query[i] ) ;
		printf( "<br>" ) ;

		if( query[i] == '\n' || query[i] == '=' )
			printf("</b>") ;
	}
}


int get_del_id(){
	int id = -1 ;

	// get query
	char *query = utility_Fetch_Query_By_POST() ;
	print_query_detail( query ) ;

	if( query ){
		// parse query
		char *seek = query ;
		while( *seek ){
			char *key, *value ;
			seek = utility_Parse_Query( seek, &key, &value, "\n" ) ;

			if( !strcmp( key, "id" ) ){
				sscanf( value, "%d", &id ) ;
				break ;
			}
		}

		free( query ) ;
	}

	return id ;
}


void web_out_head(){
	printf("<html><meta http-equiv=\"refresh\" content=\"15; url=/cgi-bin/index.exe?lastpage=del_refresh\" />\n<body>\n") ;
}


void web_out_final( int id ){
	printf("<br><b>DELETE MESSAGE #%d.</b><br>", id) ;
	printf( "<br><a href=\"/cgi-bin/index.exe?lastpage=del_hyperlink\">Redirect to index.exe</a>, or wait for 15 seconds.<br>\n" ) ;

	printf("</body></html>\n") ;
}


int main(){
	web_out_head() ;

	int id = get_del_id() ;
	db_Del( id ) ;

	web_out_final( id ) ;

	return 0 ;
}
