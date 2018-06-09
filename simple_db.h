#ifndef __SIMPLE_DB_H__
#define __SIMPLE_DB_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define DB_MSG		"db_msg.txt"
#define DB_NAME		"db_name.txt"

#define MAX_VALUE_LEN	1024

#define KEYS	3
char db_record_key[KEYS][16] = {
		"firstname",
		"lastname",
		"msg" } ;

int db_record_num = 0 ;
char ***db_record_value = NULL ;
/*
db_record_value[i][j][]: the i-th record has its j-th value to the j-th key.

You may consider using struct:
typedef struct{
	char *firstname ;
	char *lastname ;
	char *msg ;
} Record ;
Record *db_record ;

Then use:
db_record[i].firstname
db_record[i].lastname
db_record[i].msg
*/

char EMPTY_STR[] = "" ;


void db_write_all(){
	// if db is not loaded, abort.
	if( !db_record_value )
		return ;

	// open files
	FILE *fout_msg = fopen( DB_MSG, "w" ) ;
	FILE *fout_name = fopen( DB_NAME, "w" ) ;

	// write all records
	if( fout_msg && fout_name ){
		fprintf( fout_msg, "%d\n", db_record_num ) ;

		int i ;
		for( i=0 ; i<db_record_num ; i++ ){
			if( db_record_value[i] ){
				// write msg
				fprintf( fout_msg, "%s\n", db_record_value[i][2] ) ;
				// write firstname, lastname
				fprintf( fout_name, "%s\t%s\n", db_record_value[i][0], db_record_value[i][1] ) ;
			}
		}

		fclose( fout_msg ) ;
		fclose( fout_name ) ;
	}

	// clear memory
	if( db_record_value ){
		int i, j ;
		for( i=0 ; i<db_record_num ; i++ ){
			if( db_record_value[i] ){
				for( j=0 ; j<KEYS ; j++ ){
					if( db_record_value[i][j] != EMPTY_STR )
						free( db_record_value[i][j] ) ;
				}
				free( db_record_value[i] ) ;
			}
		}
		free( db_record_value ) ;
		db_record_value = NULL ;
	}
	db_record_num = 0 ;
}


void db_read_all(){
	// if db is loaded, skip.
	if( db_record_value )
		return ;

	char line[MAX_VALUE_LEN] ;

	// open files
	FILE *fin_msg = fopen( DB_MSG, "r" ) ;
	FILE *fin_name = fopen( DB_NAME, "r" ) ;

	// set db_record_num
	if( fin_msg && fin_name ){
		fgets( line, sizeof(line), fin_msg ) ;
		db_record_num = atoi(line) ;
	}

	// allocate db_record_value
	db_record_value = (char***) malloc( (db_record_num+1) * sizeof(char**) ) ;

	int i, j ;
	for( i=0 ; i<=db_record_num ; i++ )
		db_record_value[i] = NULL ;

	// read all records
	if( fin_msg && fin_name ){
		for( int i=0 ; i<db_record_num ; i++ ){
			// prepare record
			char **record = (char**) malloc( KEYS*sizeof(char*) ) ;
			db_record_value[i] = record ;

			for( j=0 ; j<KEYS ; j++ )
				record[j] = EMPTY_STR ;

			// read msg
			if( fgets( line, sizeof(line), fin_msg ) ){
				char *s ;
				for( s=line ; *s && *s!='\n' ; s++ ) ;
				if( *s )
					*s = '\0' ;

				record[2] = (char*) malloc( (strlen(line)+1)*sizeof(char) ) ;
				strcpy( record[2], line ) ;
			}

			// read name
			if( fgets( line, sizeof(line), fin_name ) ){
				char *s, *str ;

				// firstname
				for( str=s=line ; *s && *s!='\t' && *s!='\n' ; s++ ) ;
				if( *s ){
					*s = '\0' ;
					s ++ ;
				}
				record[0] = (char*) malloc( (strlen(str)+1)*sizeof(char) ) ;
				strcpy( record[0], str ) ;

				// lastname
				for( str=s ; *s && *s!='\t' && *s!='\n' ; s++ ) ;
				if( *s ){
					*s = '\0' ;
					s ++ ;
				}
				record[1] = (char*) malloc( (strlen(str)+1)*sizeof(char) ) ;
				strcpy( record[1], str ) ;
			}
		}

		fclose( fin_msg ) ;
		fclose( fin_name ) ;
	}
}


void db_show(){
	db_read_all() ;

	int i, j ;
	for( i=0 ; i < db_record_num ; i++ ){
		printf( "#%d:", i ) ;
		for( j=0 ; j<KEYS ; j++ )
			printf( " %s:\"%s\"", db_record_key[j], db_record_value[i][j] ) ;
		printf( "<br>\n" ) ;
	}
}


void db_append( const char **record_value ){
	db_read_all() ;

	char **record = (char**) malloc( KEYS*sizeof(char*) ) ;
	db_record_value[db_record_num] = record ;
	db_record_num ++ ;

	int j ;
	for( j=0 ; j<KEYS ; j++ ){
		record[j] = (char*) malloc( (strlen(record_value[j])+1)*sizeof(char) ) ;
		strcpy( record[j], record_value[j] ) ;
	}

	db_write_all() ;
}


void db_clear(){
	remove( DB_MSG ) ;
	remove( DB_NAME ) ;
}


void db_del(int id){
	db_read_all() ;

	if( id<0 || db_record_num<=id )
		return ;

	int i ;
	for( i=id+1 ; i<db_record_num ; i++ )
		db_record_value[i-1] = db_record_value[i] ;
	db_record_num -- ;

	db_write_all() ;
}

#endif
