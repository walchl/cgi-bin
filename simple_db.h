#ifndef __SIMPLE_DB_H__
#define __SIMPLE_DB_H__


#include <stdio.h>
#include <stdlib.h>


#define DB		"db.txt"
#define	TMP		"tmp.txt"

#define MAX_RECORD_LEN	1024


int db_show(){
	int count = 0 ;

	FILE *fin = fopen( DB, "r" ) ;
	if( fin ){
		char line[MAX_RECORD_LEN] ;

		for( ; fgets( line, sizeof(line), fin ) ; count++ )
			printf( "#%d: %s<br>", count, line ) ;

		fclose( fin ) ;
	}

	return count ;
}


void db_append( const char *record ){
	FILE *fout = fopen( DB, "a" ) ;
	if( fout ){
		fprintf( fout, "%s\n", record ) ;
		fclose( fout ) ;
	}
}


void db_clear(){
	remove( DB ) ;
}


void db_del(int id){
	FILE *fin = fopen( DB, "r" ) ;
	if( fin ){
		FILE *fout = fopen( TMP, "w" ) ;

		if( fout ){
			char line[MAX_RECORD_LEN] ;

			int i ;
			for( i=0 ; fgets(line, sizeof(line), fin ) ; i++ ){
				if( i != id )
					fprintf( fout, "%s", line ) ;
			}
			fclose( fout ) ;

			remove( DB ) ;
			rename( TMP, DB ) ;
		}

		fclose( fin ) ;
	}
}

#endif
