#include <stdio.h>
#include <stdarg.h>

#include <windows.h>

/***************************************************************************
Log()
*/
int Log( char *Str, ... )
{
   static char LOG_FILE[ 100 ];
   static int  FirstCall = 1;
   FILE      *Handle;
   va_list     Start;

   va_start( Start, Str );
   vprintf( Str, Start );
   printf( "\n" );

   if( FirstCall )
   {
      int   iCount = 0;

      FirstCall = 0;
      sprintf( LOG_FILE, "jetsrv%.2d.log", iCount++ );

      // Find out a new file name that can be used..
      while( access( LOG_FILE, 0 ) == 0 && iCount < 99 )
      {
         sprintf( LOG_FILE, "jetsrv%.2d.log", iCount++ );
      }

      Handle = fopen( LOG_FILE, "wb+" );
   }
   else
      Handle = fopen( LOG_FILE, "ab+" );

   if( Handle == NULL )
   {
      printf( "Cannot open file\n" );
      return( -1 );
   }
   vfprintf( Handle, Str, Start );
   fprintf( Handle, "\r\n" );
   fclose( Handle );

   return( 0 );
}

ScrambleBuffer( CHAR *Buffer, ULONG ulSize )
{
   ULONG ulCount;
   static USHORT usPos = 1;

   for( ulCount = 0; ulCount < ulSize; ulCount += usPos )
   {
      Buffer[ ulCount ] = rand();
   }

   if( usPos++ == 200 )
   		usPos = 1;

}


main()
{
   CHAR     CompareBuffer[ 16*1024 ];
   CHAR     UnCompressedBuffer[ 16*1024];
   CHAR     CompressedBuffer[ 32*1024 ];
   CHAR     WorkSpace[ 64*1024];
   ULONG    ret;
   ULONG    ulCompressedSize;
   ULONG    ulUncompressedSize;
   ULONG    ulCount;

   while( !kbhit() )
   {
      ScrambleBuffer( UnCompressedBuffer, 16*1024 );

      memcpy( CompareBuffer, UnCompressedBuffer, sizeof( CompareBuffer ));

      ret = RtlCompressBuffer ( 2, UnCompressedBuffer, sizeof( UnCompressedBuffer), 
               CompressedBuffer, sizeof( CompressedBuffer ), 1024L, &ulCompressedSize, 
               WorkSpace );

      if( ret == 0 )
      {
         memset( UnCompressedBuffer, 0, sizeof( UnCompressedBuffer ));

         ret = RtlDecompressBuffer ( 2, UnCompressedBuffer, 16*1024,
                  CompressedBuffer, ulCompressedSize, &ulUncompressedSize );

         if( ret == 0 )
         {
            if( memcmp( CompareBuffer, UnCompressedBuffer, sizeof( CompareBuffer ) ))
            {
               printf( "Comparison fails\n" );
            }
            else
            {
               printf( "Compare OK, Compressed Size %ld, Uncompressed Size %ld\n", 
                        ulCompressedSize, ulUncompressedSize );
            }
			memset( UnCompressedBuffer, 0, sizeof( UnCompressedBuffer ));

         }
         else
         {
            printf( "Decompress failes Err %d\n", ret );
			break;
         }
      }
      else
      {
         printf( "Compress Buffer Error %d\n", ret );
		 break;
      }
   }

   printf( "ret - %d", ret );
}

