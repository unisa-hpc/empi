/*
 * Copyright (c) 2004-2007 The Trustees of Indiana University and Indiana
 *                         University Research and Technology
 *                         Corporation.  All rights reserved.
 * Copyright (c) 2004-2021 The University of Tennessee and The University
 *                         of Tennessee Research Foundation.  All rights
 *                         reserved.
 * Copyright (c) 2004-2008 High Performance Computing Center Stuttgart,
 *                         University of Stuttgart.  All rights reserved.
 * Copyright (c) 2004-2005 The Regents of the University of California.
 *                         All rights reserved.
 * Copyright (c) 2015      Research Organization for Information Science
 *                         and Technology (RIST). All rights reserved.
 * $COPYRIGHT$
 *
 * Additional copyrights may follow
 *
 * $HEADER$
 */

#include "ompi_config.h"

#include "ompi/mpi/c/bindings.h"
#include "ompi/runtime/params.h"
#include "ompi/communicator/communicator.h"
#include "ompi/errhandler/errhandler.h"
#include "ompi/mca/pml/pml.h"
#include "ompi/memchecker.h"
#include "ompi/request/request.h"
#include "ompi/runtime/ompi_spc.h"

#if OMPI_BUILD_MPI_PROFILING
#if OPAL_HAVE_WEAK_SYMBOLS
#pragma weak MPI_Recv = PMPI_Recv
#endif
#define MPI_Recv PMPI_Recv
#endif

static const char FUNC_NAME[] = "MPI_Recv";


int MPI_Recv(void *buf, int count, MPI_Datatype type, int source,
             int tag, MPI_Comm comm, MPI_Status *status)
{
    int rc = MPI_SUCCESS;
    // double start1=0, start2=0, end1=0, end2=0, start3=0, end3=0, start4=0, end4=0, start5=0, end5=9;

    SPC_RECORD(OMPI_SPC_RECV, 1);

    // start1 = MPI_Wtime();
    MEMCHECKER(
        memchecker_datatype(type);
        memchecker_call(&opal_memchecker_base_isaddressable, buf, count, type);
        memchecker_comm(comm);
    );
    // end1 = MPI_Wtime();

    // start2 = MPI_Wtime();
    if ( MPI_PARAM_CHECK ) {
        OMPI_ERR_INIT_FINALIZE(FUNC_NAME);
        OMPI_CHECK_DATATYPE_FOR_RECV(rc, type, count);
        OMPI_CHECK_USER_BUFFER(rc, buf, type, count);

        if (ompi_comm_invalid(comm)) {
            return OMPI_ERRHANDLER_INVOKE(MPI_COMM_WORLD, MPI_ERR_COMM, FUNC_NAME);
        } else if (((tag < 0) && (tag != MPI_ANY_TAG)) || (tag > mca_pml.pml_max_tag)) {
            rc = MPI_ERR_TAG;
        } else if ((source != MPI_ANY_SOURCE) &&
                   (MPI_PROC_NULL != source) &&
                   ompi_comm_peer_invalid(comm, source)) {
            rc = MPI_ERR_RANK;
        }

        OMPI_ERRHANDLER_CHECK(rc, comm, rc, FUNC_NAME);
    }
    // end2 = MPI_Wtime();
    
    // start3 = MPI_Wtime();
    if (MPI_PROC_NULL == source) {
        if (MPI_STATUS_IGNORE != status) {
            OMPI_COPY_STATUS(status, ompi_request_empty.req_status, false);
        }
        return MPI_SUCCESS;
    }

    OPAL_CR_ENTER_LIBRARY();
    // end3 = MPI_Wtime();
    // start4 = MPI_Wtime();
    rc = MCA_PML_CALL(recv(buf, count, type, source, tag, comm, status));
    // end4 = MPI_Wtime();
    // MPI_Barrier(comm);
    // printf("\n----------RECV : MEMCHECKER : %lf \n", (end1 - start1)  * 1000000);
    // printf("----------RECV : MPI_PARAM_CHECK : %lf \n", (end2 - start2)  * 1000000);
    // printf("----------RECV : OTHER checks : %lf \n", (end3 - start3)  * 1000000);
    // printf("----------RECV : Receiving Data : %lf \n", (end4 - start4)  * 1000000);

    OMPI_ERRHANDLER_RETURN(rc, comm, rc, FUNC_NAME);
}
