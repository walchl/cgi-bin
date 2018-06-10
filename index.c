#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simple_db.h"
#include "utility.h"

#define FORM	"form.xml"

char _firstname[] = "First" ;
char _lastname[] = "Last" ;

const char* record[KEYS] = {0} ;

// flag for database operation
int clear_db = 0 ;
int append_db = 0 ;


// Process Query
void process_key_value( char *key, char *value ){
	// if key is "cmd"
	if( !strcmp(key, "cmd") ){
		if( !strcmp(value, "clear-db") )
			clear_db = 1 ;
		else if( !strcmp(value, "append-db") )
			append_db = 1 ;
	}
	// if key is "lastpage"
	else if( !strcmp(key, "lastpage") ){
	}
	// if key is "token"
	else if( !strcmp(key, "token") ){
	}
	// other keys
	else{
		int j ;
		for( j=0 ; j<KEYS ; j++ ){
			if( !strcmp(key, db_record_key[j]) )
				record[j] = value ;
		}
	}
}

// Process Query
void process( char *query_get, char *query_post ){
	// parse query_get
	if( query_get && *query_get ){
		char *seek = query_get ;
		while( *seek ){
			// get each (key, value) pair
			char *key, *value ;
			seek = utility_Parse_Query( seek, &key, &value, "&" ) ;

			process_key_value( key, value ) ;
		}
	}

	// parse query_post
	if( query_post && *query_post ){
		char *seek = query_post ;
		while( *seek ){
			// get each (key, value) pair
			char *key, *value ;
			seek = utility_Parse_Query( seek, &key, &value, "\n" ) ;

			process_key_value( key, value ) ;
		}
	}

	// modify db
	if( clear_db )
		db_clear() ;
	else if( append_db )
		db_append( record ) ;
}

void print_xml_form( int count ){
	char xml[1024] = {0} ;

	FILE *fin = fopen( FORM, "r" ) ;
	if( fin ){
		fscanf( fin, "%1024c", xml ) ;
		fclose( fin ) ;
	}

	if( xml[0] ){
		unsigned int token = 0 ;
		if( record[0] && record[1] )
			token = utility_Gen_Token( record[0], record[1] ) ;

		if( !record[0] )
			record[0] = _firstname ;
		if( !record[1] )
			record[1] = _lastname ;
		printf( xml, record[0], record[1], token, count ) ;
	}
}


void web_out(){
	printf("<html>\n<body>\n") ;

	// process query
	char *query_get = utility_Fetch_Query_By_GET() ;
	char *query_post = utility_Fetch_Query_By_POST() ;

	printf( "%s<br>\"%s\"<br><br>\n", "QUERY_STRING(GET)", query_get?query_get:"(NULL)" ) ;
	printf( "%s<br>\"%s\"<br><br>\n", "QUERY_STRING(POST)", query_post?query_post:"(NULL)" ) ;

	process( query_get, query_post ) ;

	// show database
	db_show() ;

	// print forms
	print_xml_form( db_record_num ) ;

	printf("</body>\n</html>\n") ;
}


int main(){
	web_out() ;

	return 0 ;
}
