// Copyright 2020 Galindo Buitrago Julio Cesar

#include "../../../../modules/task_2/galindo_j_barbero_dormilon/barbero_dormilon.h"
#include <mpi.h>
#if defined(WIN32)
  #include <windows.h>
#elif defined(__unix__)
  #include <time.h>
  #include <unistd.h>
#elif defined(__APPLE__)
  #include <unistd.h>
#else
#endif
#include <iostream>

int millisleep(unsigned ms) {
#if defined(WIN32)
  SetLastError(0);
  Sleep(ms);
  return GetLastError() ?-1 :0;
#elif _POSIX_C_SOURCE >= 199309L
  const struct timespec ts = {
    ms / 1000, /* seconds */
    (ms % 1000) * 1000 * 1000 /* nano seconds */
  };
  return nanosleep(&ts, NULL);
#elif _BSD_SOURCE || \
  (_XOPEN_SOURCE >= 500 || \
     _XOPEN_SOURCE && _XOPEN_SOURCE_EXTENDED) && \
  !(_POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700)
  return usleep(1000 * ms);
#elif __APPLE__
  return usleep(1000 * ms);
#else
# error("No millisecond sleep available for this platform!")
  return -1;
#endif
}

void Cliente::inicio()
{   MPI_Status status;
    Mensaje mensaje;
    mensaje.orden = 1;
    mensaje.id_proc = id_proc;
    printf("[C]-[%d] \n", id_proc);
    fflush(stdout);
    while (-2 != mensaje.orden) {
        mensaje.orden = 1;
        MPI_Send(&mensaje, sizeof(Mensaje), MPI_CHAR, id_fila, 123, MPI_COMM_WORLD);
        MPI_Recv(&mensaje, sizeof(Mensaje), MPI_CHAR, id_fila, 123, MPI_COMM_WORLD, &status);
        switch (mensaje.orden) {
            case -2:
            {
                break;
            }
            case -1:
            {
                millisleep(500);
                break;
            }
            default:
            {   // 1.se envia informacion hacia la fila.
                // 2.se hacen cambios dictados por el barbero.
                MPI_Send(this, sizeof(Cliente), MPI_CHAR, id_fila, 123, MPI_COMM_WORLD);
                MPI_Recv(this, sizeof(Cliente), MPI_CHAR, id_barbero, 10, MPI_COMM_WORLD, &status);
                printf("[C]-[%d] obtuvo su nuevo corte de cabello\n", id_proc);
                //
                MPI_Send(&mensaje, sizeof(Mensaje), MPI_CHAR, id_barbero, 10, MPI_COMM_WORLD);
            }
        }
    }
    printf("[C]-[%d] se fue a casa\n", id_proc);
    fflush(stdout);
}
void Cliente::corte() {
}
int Fila::revisar_fila() {
    if (0 == longitud) {
            return -2;  // final del dia de trabajo.
    }
    if (0 == num_clientes) {
        return 1;  // revisar las acciones del barbero.
    }
    if (num_clientes < max_clientes) {
        return 0;   //  estar en la fila.
    }
    if (num_clientes >= max_clientes) {
        return -1;  //  nada que hacer.
    }
    return -1;
}
void Fila::inicio() {
    // 1.comunicacion con el barbero.
    // 2.principal loop.
    printf("[F] empezo con el num_proc_ejec = %d y el max_clientes en la fila = %d\n", num_proc_ejec, max_clientes);
    fflush(stdout);
    MPI_Status status;
    Mensaje mensaje;
    //
    MPI_Recv(&mensaje.orden, 1, MPI_INT, id_barbero, 1, MPI_COMM_WORLD, &status);
    MPI_Send(&mensaje.orden, 1, MPI_INT, id_barbero, 1, MPI_COMM_WORLD);
    //
    while ((num_proc_ejec > 0) || (!barbero_durmiendo)) {
        MPI_Recv(&mensaje, sizeof(Mensaje), MPI_CHAR, MPI_ANY_SOURCE, 123, MPI_COMM_WORLD, &status);
        printf("[F] obtuvo una peticion del %d proc con la orden = %d\n", mensaje.id_proc, mensaje.orden);
        fflush(stdout);
        // ordenes generadas hacia los clientes.
        switch (mensaje.orden) {
            case 1:
            {   mensaje.orden = revisar_fila();
                if (-2 == mensaje.orden) {
                    num_proc_ejec--;
                }
                MPI_Send(&mensaje, sizeof(Mensaje), MPI_CHAR, mensaje.id_proc, 123, MPI_COMM_WORLD);
                if (mensaje.orden >= 0) {
                    longitud--;
                    num_clientes++;
                    MPI_Recv(&ClienteFila[longitud],
                        sizeof(Cliente), MPI_CHAR, mensaje.id_proc, 123, MPI_COMM_WORLD, &status);
                    ClienteFila[longitud].lugar_enfila = longitud;
                    printf("[F] con el proc id = %d en el %d lugar\n", ClienteFila[longitud].getId_Proc(), longitud);
                    fflush(stdout);
                    if ((1 == mensaje.orden) && (barbero_durmiendo)) {
                        printf("[F] yendo a despertar al barbero\n");
                        fflush(stdout);
                        MPI_Send(&mensaje.orden, 1, MPI_INT, id_barbero, 100, MPI_COMM_WORLD);
                        MPI_Recv(&mensaje.orden, 1, MPI_INT, id_barbero, 100, MPI_COMM_WORLD, &status);
                        barbero_durmiendo = false;
                    }
                }
                break;
            }
            case 2:
            {   // esta vacia la orden del barbero (pregunta)
                mensaje.orden = num_clientes;
                MPI_Send(&mensaje, sizeof(Mensaje), MPI_CHAR, id_barbero, 1, MPI_COMM_WORLD);
                if (mensaje.orden > 0) {
                    // obtener la respuesta del barbero
                    MPI_Send(&ClienteFila[longitud + num_clientes - 1],
                        sizeof(Cliente), MPI_CHAR, id_barbero, 2, MPI_COMM_WORLD);
                    //
                    MPI_Recv(&mensaje, sizeof(Mensaje), MPI_CHAR, id_barbero, 1, MPI_COMM_WORLD, &status);
                    num_clientes--;
                } else {
                    barbero_durmiendo = true;
                }
                break;
            }
        }
    }
    if (barbero_durmiendo) {
        printf("[F] yendo a despertar al barbero\n");
        fflush(stdout);
        mensaje.orden = -2;
        MPI_Send(&mensaje.orden, 1, MPI_INT, id_barbero, 100, MPI_COMM_WORLD);
        MPI_Recv(&mensaje.orden, 1, MPI_INT, id_barbero, 100, MPI_COMM_WORLD, &status);
        printf("[F] el barbero se despierta (%d)\n", mensaje.orden);
        fflush(stdout);
    }
    free(ClienteFila);
    printf("[F] termino su trabajo\n");
    fflush(stdout);
}
void Barbero::inicio() {
    Cliente ClienteActual;
    MPI_Status status;
    Mensaje mensaje;
    mensaje.orden = 2;
    mensaje.id_proc = id_proc;
    // conectarse con la fila
    MPI_Send(&mensaje.orden, 1, MPI_INT, id_fila, 1, MPI_COMM_WORLD);
    MPI_Recv(&mensaje.orden, 1, MPI_INT, id_fila, 1, MPI_COMM_WORLD, &status);
    while (mensaje.orden != -2) {
        mensaje.orden = 2;
        MPI_Send(&mensaje, sizeof(Mensaje), MPI_CHAR, id_fila, 123, MPI_COMM_WORLD);
        MPI_Recv(&mensaje, sizeof(Mensaje), MPI_CHAR, id_fila, 1, MPI_COMM_WORLD, &status);
        if (mensaje.orden > 0) {
            MPI_Recv(&ClienteActual, sizeof(Cliente), MPI_CHAR, id_fila, 2, MPI_COMM_WORLD, &status);
            // para la sincronizacion, se envia respuesta a la fila.
            MPI_Send(&mensaje, sizeof(Mensaje), MPI_CHAR, id_fila, 1, MPI_COMM_WORLD);
            printf("[B] inicia [%d] y con el lugar = *%d*\n", ClienteActual.getId_Proc(), ClienteActual.lugar_enfila);
            fflush(stdout);
            ClienteActual.corte();
            MPI_Send(&ClienteActual, sizeof(Cliente), MPI_CHAR, ClienteActual.getId_Proc(), 10, MPI_COMM_WORLD);
            // para la sincronizacion, se recibe la respuesta desde el cliente.
            MPI_Recv(&mensaje, sizeof(Mensaje), MPI_CHAR, ClienteActual.getId_Proc(), 10, MPI_COMM_WORLD, &status);
            mensaje.id_proc = id_proc;
        } else if (mensaje.orden != -2) {
            printf("[B] se fue a dormir\n");
            fflush(stdout);
            MPI_Recv(&mensaje.orden, 1, MPI_INT, id_fila, 100, MPI_COMM_WORLD, &status);
            printf("[B] se desperto (%d)\n", mensaje.orden);
            fflush(stdout);
            MPI_Send(&mensaje.orden, 1, MPI_INT, id_fila, 100, MPI_COMM_WORLD);
        }
    }
    printf("[B] termino su trabajo\n");
    fflush(stdout);
}
void ejecucion() {
    int num_proc, id_proc;
    MPI_Comm_size(MPI_COMM_WORLD, &num_proc);
    MPI_Comm_rank(MPI_COMM_WORLD, &id_proc);
    if (num_proc <= 2) {
        throw "punto de prueba";
    } else if (id_proc == 0) {
        Barbero barbero(0, 1);
        barbero.inicio();
    } else if (id_proc == 1) {
        Fila fila(1, 30, 0, num_proc);
        fila.inicio();
    } else {
        Cliente cliente(id_proc, 1, 0);
        cliente.inicio();
    }
}
