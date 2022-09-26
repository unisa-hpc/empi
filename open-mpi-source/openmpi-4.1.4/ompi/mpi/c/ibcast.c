/*
 * Copyright (c) 2012      Oak Rigde National Laboratory. All rights reserved.
 * Copyright (c) 2015-2019 Research Organization for Information Science
 *                         and Technology (RIST).  All rights reserved.
 * Copyright (c) 2017-2020 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */
#include "ompi_config.h"
#include <stdio.h>

#include "ompi/mpi/c/bindings.h"
#include "ompi/runtime/params.h"
#include "ompi/communicator/communicator.h"
#include "ompi/errhandler/errhandler.h"
#include "ompi/datatype/ompi_datatype.h"
#include "ompi/mca/coll/base/coll_base_util.h"
#include "ompi/memchecker.h"
#include "ompi/runtime/ompi_spc.h"

#if OMPI_BUILD_MPI_PROFILING
#if OPAL_HAVE_WEAK_SYMBOLS
#pragma weak MPI_Ibcast = PMPI_Ibcast
#endif
#define MPI_Ibcast PMPI_Ibcast
#endif

static const char FUNC_NAME[] = "MPI_Ibcast";


int MPI_Ibcast(void *buffer, int count, MPI_Datatype datatype,
              int root, MPI_Comm comm,  MPI_Request *request)
{
    int err;
    // double start1 = 0.0, end1 = 0.0, start2 = 0.0, end2 = 0.0, start3 = 0.0, end3 = 0.0, start4=0.0, end4=0.0, start5=0.0, end5=0.0;

    SPC_RECORD(OMPI_SPC_IBCAST, 1);
    // printf("\n--------------------------------IBCAST");
    // start1 = MPI_Wtime();
    MEMCHECKER(
        memchecker_datatype(datatype);
        memchecker_call(&opal_memchecker_base_isdefined, buffer, count, datatype);
        memchecker_comm(comm);
    );
    // end1 = MPI_Wtime();

    // start2 = MPI_Wtime();
    if (MPI_PARAM_CHECK) {
      err = MPI_SUCCESS;
      OMPI_ERR_INIT_FINALIZE(FUNC_NAME);
      if (ompi_comm_invalid(comm)) {
          return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, MPI_ERR_COMM,
                                     FUNC_NAME);
      }

      /* Errors for all ranks */

      OMPI_CHECK_DATATYPE_FOR_SEND(err, datatype, count);
      OMPI_ERRHANDLER_CHECK(err, comm, err, FUNC_NAME);
      if (MPI_IN_PLACE == buffer) {
          return OMPI_ERRHANDLER_INVOKE(comm, MPI_ERR_ARG, FUNC_NAME);
      }

      /* Errors for intracommunicators */

      if (OMPI_COMM_IS_INTRA(comm)) {
        if ((root >= ompi_comm_size(comm)) || (root < 0)) {
          return OMPI_ERRHANDLER_INVOKE(comm, MPI_ERR_ROOT, FUNC_NAME);
        }
      }

      /* Errors for intercommunicators */

      else {
        if (! ((root >= 0 && root < ompi_comm_remote_size(comm)) ||
               MPI_ROOT == root || MPI_PROC_NULL == root)) {
            return OMPI_ERRHANDLER_INVOKE(comm, MPI_ERR_ROOT, FUNC_NAME);
        }
      }
    }
    // end2 = MPI_Wtime();

    /* If there's only one node, or if the count is 0, we're done */
    // start3 = MPI_Wtime();
    if ((OMPI_COMM_IS_INTRA(comm) && ompi_comm_size(comm) <= 1) ||
        0 == count) {
        *request = &ompi_request_empty;
        return MPI_SUCCESS;
    }

    OPAL_CR_ENTER_LIBRARY();
    // end3 = MPI_Wtime();
    /* Invoke the coll component to perform the back-end operation */
    // start4 = MPI_Wtime();
    err = comm->c_coll->coll_ibcast(buffer, count, datatype, root, comm,
                                  request,
                                  comm->c_coll->coll_ibcast_module);
    // end4 = MPI_Wtime();
    // start5 = MPI_Wtime();
    if (OPAL_LIKELY(OMPI_SUCCESS == err)) {
        if (!OMPI_COMM_IS_INTRA(comm)) {
            if (MPI_PROC_NULL == root) {
                datatype = NULL;
            }
        }
        ompi_coll_base_retain_datatypes(*request, datatype, NULL);
    }
  //   end5 = MPI_Wtime();
  // if (ompi_comm_rank(comm) == root) {
  //   // MPI_Barrier(comm);
  //   printf("\nIBCAST : MEMCHECKER : %lf \n", ((end1 - start1))  * 1000000);
  //   printf("IBCAST : MPI_PARAM_CHECK : %lf \n", (end2 - start2)  * 1000000);
  //   printf("IBCAST : OTHER checks : %lf \n", (end3 - start3) + (end5-start5)  * 1000000);
  //   printf("IBCAST : Sending Data : %lf \n", (end4 - start4)  * 1000000);
  // }
    OMPI_ERRHANDLER_RETURN(err, comm, err, FUNC_NAME);
}
