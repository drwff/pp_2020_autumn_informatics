// Copyright 2020 Galindo Buitrago Julio Cesar
#ifndef MODULES_TASK_2_GALINDO_J_BARBERO_DORMILON_BARBERO_DORMILON_H_
#define MODULES_TASK_2_GALINDO_J_BARBERO_DORMILON_BARBERO_DORMILON_H_

typedef struct Mensaje_o {
    int orden;
    int id_proc;
} Mensaje;

class Cliente{
    int id_proc;
    int id_fila;
    int id_barbero;
 public:
        int lugar_enfila;
        explicit Cliente(int id_proc = -1, int id_fila = 1, int id_barbero = 0) :
            id_proc(id_proc),
            id_fila(id_fila),
            id_barbero(id_barbero) {}
        int enEspera();
        void inicio();
        void corte();
        int getId_Proc() {
            return id_proc;
        }
};
class Fila{
    Cliente *ClienteFila;
    //  cantidad de clientes en general.
    int longitud;
    //  clientes actuales en la barberia.
    int num_clientes;
    //  max en la fila.
    int max_clientes;
    int id_proc;
    int num_proc_ejec;
    bool barbero_durmiendo;
    int id_barbero;

 public:
        explicit Fila(int id_proc = 1, int longitud = 100, int id_barbero = 0, int num_proc_ejec = 2) {
            if (longitud > 0) {
                this->id_proc = id_proc;
                this->longitud = longitud;
                this->id_barbero = id_barbero;
                this->num_proc_ejec = num_proc_ejec - 2;
                ClienteFila = new Cliente[longitud];
                num_clientes = 0;
                max_clientes = longitud/2;
                barbero_durmiendo = false;
            } else {
                throw "la longitud debe ser > 0";
            }
        }
        int revisar_fila();
        void inicio();
};
class Barbero{
    int id_proc;
    int id_fila;
 public:
        explicit Barbero(int id_proc = 0, int id_fila = 1) :
            id_proc(id_proc),
            id_fila(id_fila) {}
        void inicio();
        bool despertar();
};
void ejecucion();
#endif  // MODULES_TASK_2_GALINDO_J_BARBERO_DORMILON_BARBERO_DORMILON_H_
