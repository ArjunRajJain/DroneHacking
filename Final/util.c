#include "demo.h"

/*
 * lookup function -->
 * returns the "value" part of a <name, value> pair when given "name"; NULL if not found.
 * Here it returns the password for the given username
 */
char *lookup( name )
  char *name;
  {
    struct sym_list *sp;

    sp = s_lookup( name );
    if( sp != (struct sym_list *) NULL )
      return( sp->s_sym.v_value );
    else
      return( (char *) NULL );
  }

/*
 * insert() function -->
 * inserts a new <name,value> pair into the database file which is then stored
 */
insert( name, value )
  char *name, *value;
  {
    struct sym_list *sp;
    sp = s_lookup( name );

    /* if the name exists in the database file, nothing is changed.
     * Otherwise, a new element is inserted at the beginning of the list in the file. 
    */

    if( sp == (struct sym_list * ) NULL )
      {
        sp = new_sym();
        if( sp == (struct sym_list *) NULL )
  	     {
      	  printf( "Fatal error: no memory\n" );
      	  exit( 1 );
  	     }
  	    sp->s_next = Head.s_next;
        Head.s_next = sp;
      }

    /* one way or another, "sp" now points to a valid entry. */
    sp->s_sym.v_name = strsave( name );
    sp->s_sym.v_value = strsave( value );
	}

/* strsave()
 * returns a pointer to a copy of the 
 * character string pointed to by "str".
 */
char *strsave( str )
  char *str;
  {
  	char *p;

  	p = (char *) malloc( strlen(str)+1 );
  	if( p != (char *) NULL )
  		strcpy( p, str );
  	return( p );
  }

/*new_sym()
 * provides a pointer to a new sym_list structure
 */
struct sym_list *new_sym()
{
  struct sym_list *p;

  p = (struct sym_list *) malloc( sizeof( struct sym_list ) );

  /* on failure, malloc() returns NULL,
   * which is our error return as well.
   */
  return( p );
	
}

/* s_lookup:
 * finds a symlist structure on the list pointed to by Head which has a v_name entry matching name.
 */

struct sym_list *s_lookup( name )
  char *name;
  {
    struct sym_list *sp;


    for( sp = Head.s_next; sp != (struct sym_list *) NULL; sp = sp->s_next)
      {
        if( strcmp( sp->s_sym.v_name, name ) == 0 )
  	return( sp );
      }

    return( (struct sym_list *) NULL );
  }

/* save() -
 *  - saves the set of <name, value> pairs in the database file
 */

save( file )
  char *file;
  {
    FILE *filep;
    struct sym_list *sp;

    filep = fopen( file, "w" );

    if( filep == (FILE *) NULL )
      {
        fputs( "Fatal error. Cannot Save", stderr );
        exit( 1 );
      }

    for( sp = Head.s_next;
         sp != (struct sym_list *) NULL;
         sp = sp->s_next )
      {
        fprintf( filep, "%s%c%s",
  	       sp->s_sym.v_name,
  	       EQUALS,
  	       sp->s_sym.v_value );
      }

    fclose( filep );
  }

/* restore()
 *  - restores the database file with the contents entered by the user.
 */

void restore( file )
  char *file;
  {
    FILE *db;
    char buf[BUFSIZE];

    db = fopen( file, "r" );

    if( db != (FILE *) NULL )
      {
        while( fgets( buf, BUFSIZE, db ) > 0 )
  	{
  	  char *ptr;
  	      
  	  if( (ptr = find_equals( buf ) ) > 0 )
  	    {
  	      *ptr = EOS;
  	      insert( strsave( buf), strsave( ++ptr ));
  	    }
  	}
        fclose( db );
      }
    return;
}