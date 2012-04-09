/*
  This file is part of the SC Library.
  The SC Library provides support for parallel scientific applications.

  Copyright (C) 2010 The University of Texas System

  The SC Library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  The SC Library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with the SC Library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
*/

#ifndef SC_IO_H
#define SC_IO_H

#include <sc.h>
#include <sc_containers.h>

SC_EXTERN_C_BEGIN;

typedef enum
{
  SC_IO_MODE_WRITE,     /**< Semantics as "w" in fopen. */
  SC_IO_MODE_APPEND,    /**< Semantics as "a" in fopen. */
  SC_IO_MODE_LAST       /**< Invalid entry to close list */
}
sc_io_mode_t;

typedef enum
{
  SC_IO_ENCODE_NONE,
  SC_IO_ENCODE_LAST     /**< Invalid entry to close list */
}
sc_io_encode_t;

typedef enum
{
  SC_IO_SINK_BUFFER,
  SC_IO_SINK_FILENAME,
  SC_IO_SINK_FILEFILE,
  SC_IO_SINK_LAST       /**< Invalid entry to close list */
}
sc_io_sink_type_t;

typedef struct sc_io_sink
{
  sc_io_sink_type_t   stype;
  sc_io_mode_t        mode;
  sc_io_encode_t      encode;
  sc_array_t         *buffer;
  FILE               *file;
  size_t              bytes_in;
  size_t              bytes_out;
}
sc_io_sink_t;

/** Create a generic data sink.
 * \param [in] stype            Type of the sink.
 *                              Depending on stype, varargs must follow:
 *                              BUFFER: sc_array_t * (existing non-view array).
 *                              FILENAME: const char * (name of file to open).
 *                              FILEFILE: FILE * (file open for writing).
 * \param [in] mode             Mode to add data to sink.
 *                              For type FILEFILE, data is always appended.
 * \param [in] encode           Type of data encoding.
 * \return                      Newly allocated sink, or NULL on error.
 */
sc_io_sink_t       *sc_io_sink_new (sc_io_sink_type_t stype,
                                    sc_io_mode_t mode,
                                    sc_io_encode_t encode, ...);

/** Free data sink.
 * Calls sc_io_sink_flush and discards the final output byte count.
 * Call sc_io_sink_flush yourself if bytes_out is of interest.
 * \param [in] sink             The sink object to flush and free.
 * \return                      0 on success, nonzero on error.
 */
int                 sc_io_sink_destroy (sc_io_sink_t * sink);

/** Write data to a sink.
 * \param [in,out] sink         The sink object to write to.
 * \param [in] data             Data passed into sink.
 * \param [in] bytes            Number of data bytes passed in.
 * \return                      0 on success, nonzero on error.
 */
int                 sc_io_sink_write (sc_io_sink_t * sink,
                                      const void *data, size_t bytes);

/** Flush all buffered output data to sink.
 * The updated value of bytes read and written is returned in bytes_in/out.
 * The sink status is reset as if the sink has just been created.
 * In particular, the bytes counters are reset to zero.
 * The sink actions taken depend on its type.
 * BUFFER, FILEFILE: none.
 * FILENAME: call fclose on sink->file.
 * \param [in,out] sink         The sink object to write to.
 * \param [out] bytes_in        Bytes received since the last new or flush
 *                              call.  May be NULL.
 * \param [out] bytes_in        Bytes written since the last new or flush
 *                              call.  May be NULL.
 * \return                      0 if flush is completed, nonzero on error.
 */
int                 sc_io_sink_flush (sc_io_sink_t * sink,
                                      size_t * bytes_in, size_t * bytes_out);

/** This function writes numeric binary data in VTK base64 encoding.
 * \param vtkfile        Stream openened for writing.
 * \param numeric_data   A pointer to a numeric data array.
 * \param byte_length    The length of the data array in bytes.
 * \return               Returns 0 on success, -1 on file error.
 */
int                 sc_vtk_write_binary (FILE * vtkfile, char *numeric_data,
                                         size_t byte_length);

/** This function writes numeric binary data in VTK compressed format.
 * \param vtkfile        Stream openened for writing.
 * \param numeric_data   A pointer to a numeric data array.
 * \param byte_length    The length of the data array in bytes.
 * \return               Returns 0 on success, -1 on file error.
 */
int                 sc_vtk_write_compressed (FILE * vtkfile,
                                             char *numeric_data,
                                             size_t byte_length);

/** Write memory content to a file.
 * \param [in] ptr      Data array to write to disk.
 * \param [in] size     Size of one array member.
 * \param [in] nmemb    Number of array members.
 * \param [in,out] file File pointer, must be opened for writing.
 * \param [in] errmsg   Error message passed to SC_CHECK_ABORT.
 * \note                This function aborts on file errors.
 */
void                sc_fwrite (const void *ptr, size_t size,
                               size_t nmemb, FILE * file, const char *errmsg);

/** Read file content into memory.
 * \param [out] ptr     Data array to read from disk.
 * \param [in] size     Size of one array member.
 * \param [in] nmemb    Number of array members.
 * \param [in,out] file File pointer, must be opened for reading.
 * \param [in] errmsg   Error message passed to SC_CHECK_ABORT.
 * \note                This function aborts on file errors.
 */
void                sc_fread (void *ptr, size_t size,
                              size_t nmemb, FILE * file, const char *errmsg);

#ifdef SC_MPIIO

/** Write memory content to an MPI file.
 * \param [in,out] mpifile      MPI file object opened for writing.
 * \param [in] ptr      Data array to write to disk.
 * \param [in] zcount   Number of array members.
 * \param [in] t        The MPI type for each array member.
 * \param [in] errmsg   Error message passed to SC_CHECK_ABORT.
 * \note                This function aborts on MPI file and count errors.
 */
void                sc_mpi_write (MPI_File mpifile, const void *ptr,
                                  size_t zcount, MPI_Datatype t,
                                  const char *errmsg);

#endif

SC_EXTERN_C_END;

#endif /* SC_IO_H */
