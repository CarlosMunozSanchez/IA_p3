#ifndef __AI_PLAYER_H__
#define __AI_PLAYER_H__

# include "Attributes.h"
# include "Player.h"

class AIPlayer: public Player{
    protected:
        //Id identificativo del jugador
        const int id;
    public:
        /**
         * @brief Constructor de un objeto AIPlayer
         * 
         * @param name Nombre del jugador
         */
        inline AIPlayer(const string & name):Player(name), id(0){};
        
        /**
         * @brief Constructor de un objeto AIPlayer 
         * 
         * @param name Nombre  del jugador
         * @param id Id del jugador
         */
        inline AIPlayer(const string & name, const int id):Player(name), id(id){};

        /**
         * @brief Función que percibe el el parchís y al jugador actual.
         * Asigna el tablero en actual y el id del jugador.
         * 
         * @param p Instancia Parchis que se quiere percibir
         */
        inline virtual void perceive(Parchis &p){Player::perceive(p);}

        /**
         * @brief Función abstracta que define el movimiento devuelto por el jugador.
         * Llama a la función movePiece con el valor asignado a los parámetros pasados 
         * por referencia.
         * 
         * @return true
         * @return false 
         */
        virtual bool move();
        
        /**
         * @brief Función que se encarga de decidir el mejor movimiento posible a 
         * partir del estado actual del tablero. Asigna a las variables pasadas por
         * referencia el valor de color de ficha, id de ficha y dado del mejor movimiento.
         * 
         * @param c_piece Color de la ficha
         * @param id_piece Id de la ficha
         * @param dice Número de dado
         */
        virtual void think(color & c_piece,  int & id_piece, int & dice) const;

        /**
         * @brief Método que determina si el player es inteligente (decide el mejor movimiento)
         * o no. True para AIPlayer.
         * 
         * @return true 
         * @return false 
         */
        inline virtual bool canThink() const{return true;}

        /**
         * @brief Heurística de prueba para validar el algoritmo de búsqueda.
         * 
         * @param estado Instancia de Parchis con el estado actual de la partida.
         * @param jugador Id del jugador actual (0 o 1)
         * @return double 
         */
        static double ValoracionTest(const Parchis &estado, int jugador);
        
        /**
         * @brief Heurística final del jugador inteligente
         * 
         * @param estado Instancia de Parchis con el estado actual de la partida.
         * @param jugador Id del jugador actual (0 o 1)
         * @return double 
         */
        static double GrandMaster2(const Parchis &estado, int jugador);
        
        void thinkAleatorio(color & c_piece, int & id_piece, int & dice) const;
        
        void thinkAleatorioMasInteligente(color & c_piece, int & id_piece, int & dice) const;
        
        void thinkFichaMasAdelantada(color & c_piece, int & id_piece, int & dice) const;
        
		  void thinkMejorOpcion(color & c_piece, int & id_piece, int & dice) const;

        /**
         * @brief función de búsqueda.
			* @param actual Instancia de Parchis con el estado actual de la partida
			* @param jugador Indice del jugador al que le toca jugar
			* @param profundidad Para la llamada recursiva, indicador de la profundidad en la que se encuentra
			* @param profundidad_max Límite de profundidad
			* @param c_piece Color de la pieza a la que le toca mover
			* @param id_piece Id de la pieza que debe mover
			* @param dice Tirada que se va a efectuar
			* @param alpha, beta Cotas para poda alfa-beta
			* @param (*heuristic)(const Parchis &, int) función heurística
         */
        double busqueda(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristic)(const Parchis &, int)) const;
};
#endif
