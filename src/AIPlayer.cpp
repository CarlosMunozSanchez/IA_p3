# include "AIPlayer.h"
# include "Parchis.h"

//#include <thread>
//#include <chrono>

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 4;
const int num_colors = 4;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta

const int final_red_box = 34;
const int final_blue_box = 17;
const int final_green_box = 51;
const int final_yellow_box = 68;

const int num_casillas = 68;

bool AIPlayer::move(){
    cout << "Realizo un movimiento automatico" << endl;
    
    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

void AIPlayer::thinkAleatorio(color & c_piece, int & id_piece, int & dice) const{

	    // IMPLEMENTACIÓN INICIAL DEL AGENTE
    // Esta implementación realiza un movimiento aleatorio.
    // Se proporciona como ejemplo, pero se debe cambiar por una que realice un movimiento inteligente 
    //como lo que se muestran al final de la función.
    
    // OBJETIVO: Asignar a las variables c_piece, id_piece, dice (pasadas por referencia) los valores, 
    //respectivamente, de:
    // - color de ficha a mover
    // - identificador de la ficha que se va a mover
    // - valor del dado con el que se va a mover la ficha.

    // El color de ficha que se va a mover
    c_piece = actual->getCurrentColor();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<int> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableDices(c_piece);
    // Elijo un dado de forma aleatoria.
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(c_piece, dice);

    // Si tengo fichas para el dado elegido muevo una al azar.
    if(current_pieces.size() > 0){
        id_piece = current_pieces[rand() % current_pieces.size()];
    }
    else{
        // Si no tengo fichas para el dado elegido, pasa turno (la macro SKIP_TURN me permite no mover).
        id_piece = SKIP_TURN;
    }

}

void AIPlayer::thinkAleatorioMasInteligente(color & c_piece, int & id_piece, int & dice) const{

	// El color de ficha que se va a mover
    c_piece = actual->getCurrentColor();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;
    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido.
    vector<int> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movimiento
    current_dices = actual->getAvailableDices(c_piece);

	//En vez de elegir un dado al azar, miro primero cuáles tienen fichas que se puedan mover
	vector<int> current_dices_que_pueden_mover_ficha;
   for(int i = 0; i < current_dices.size(); i++){
   	//Se obtiene el vector de fichas que se pueden mover para el dado
   	current_pieces = actual->getAvailablePieces(c_piece, current_dices[i]);
   	
   	//si se pueden mover fichas para el dadoa actual, lo añado al vector de dados que pueden mover fichas
   	if(current_pieces.size() > 0){
   		current_dices_que_pueden_mover_ficha.push_back(current_dices[i]);
   	} 
   }
   
  //Si no hay dados con los que mover ficha, hay que pasar turno con un dado al azar
  if(current_dices_que_pueden_mover_ficha.size() == 0){
  		dice = current_dices[rand() % current_dices.size()];
  		
  		id_piece = SKIP_TURN;
  }
  else{
  		dice = current_dices_que_pueden_mover_ficha[rand() % current_dices_que_pueden_mover_ficha.size()];
  		
  		//Se obtiene el vactor de fichas que se pueden mover para el dado elegido
  		current_pieces = actual->getAvailablePieces(c_piece, dice);
  		
  		//Muevo una ficha al azar de entre las que se pueden mover
  		id_piece = current_pieces[rand() % current_pieces.size()];
  }   

}

void AIPlayer::thinkFichaMasAdelantada(color & c_piece, int & id_piece, int & dice) const{
	thinkAleatorioMasInteligente(c_piece, id_piece, dice);
	
	vector<int> current_pieces = actual->getAvailablePieces(c_piece, dice);
	
	int id_ficha_mas_adelantada = -1;
	int min_distancia_meta = 9999;
	for(int i = 0; i < current_pieces.size(); i++){
		int distancia_meta = actual->distanceToGoal(c_piece, current_pieces[i]);
		if(distancia_meta < min_distancia_meta){
			min_distancia_meta = distancia_meta;
			id_ficha_mas_adelantada = current_pieces[i];
		}
	}
	
	//si no he encontrado ninguna ficha, paso turno
	if(id_ficha_mas_adelantada == -1){
		id_piece = SKIP_TURN;
	}
	else{
		id_piece = id_ficha_mas_adelantada;
	}
}

void AIPlayer::thinkMejorOpcion(color & c_piece, int & id_piece, int & dice) const{
	
	color last_c_piece = none;
	int last_id_piece = -1;
	int last_dice = -1;
	
	Parchis siguiente_hijo = actual->generateNextMove(last_c_piece, last_id_piece, last_dice);
	
	bool decidir_accion = false;
	
	while(!(siguiente_hijo == *actual) and !decidir_accion){
		if(siguiente_hijo.isEatingMove() or
			siguiente_hijo.isGoalMove() or
			(siguiente_hijo.gameOver() and siguiente_hijo.getWinner() == this->jugador)){
			
			decidir_accion = true;
		}
		else{
			siguiente_hijo = actual->generateNextMove(last_c_piece, last_id_piece, last_dice);
		}
	}
	
	if(decidir_accion){
		c_piece = last_c_piece;
		id_piece = last_id_piece;
		dice = last_dice;
	}
	else{
		thinkFichaMasAdelantada(c_piece, id_piece, dice);
	}
}

//Funciones para los algoritmos de búsqueda

double AIPlayer::busqueda(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristic)(const Parchis &, int)) const {
	
	//cout << "Explorando con alfa-beta. Profundidad: " << profundidad << endl;
	
	//valor de la heurística asociada al nodo
	double v = jugador == 0 ? menosinf : masinf;
	bool podar = false;
	
	//variables para almacenamiento de movimientos
	color color_p = none;
	int id_p = -1;
	int d = -1;
	
	Parchis siguiente_hijo;
	
	int i = 0;
	//Si no es un nodo terminal, lo exploro
	if(profundidad < profundidad_max and !actual.gameOver()){
		//genero todos los hijos que pueda
		while(!(siguiente_hijo == actual) and !podar){
			double v_aux;
			int next_j;
			
			//cout << "Hijos generados por este padre a profundidad " << profundidad << ": " << i++ << endl;
	
			//Nuevo hijo. Las var. se actualizan al movimiento efectuado
			siguiente_hijo = actual.generateNextMoveDescending(color_p, id_p, d);			
			
			next_j = siguiente_hijo.getCurrentPlayerId();
			
			//Antes de explorarlo, compruebo que no he vuelto al nodo actual
			if(!(siguiente_hijo == actual)){
				//Explorar el siguiente hijo
				v_aux = busqueda(siguiente_hijo, next_j, profundidad+1, profundidad_max, color_p, id_p, d, alpha, beta, heuristic);
				
				//Actualizar el valor de la heurística del nodo
				if((jugador == 0 and v_aux > v) or (jugador == 1 and v_aux < v)){
					v = v_aux;
					
					if(jugador == 0){
						if(v >= alpha){
							alpha = v;
						}
						
						if(alpha >= beta){
							v = beta;
							podar = true;
							//cout << "Podando..." << endl;
						}
					}
					else{
						if(v <= beta){							
							beta = v;
						}
						if(beta <= alpha){
							v = alpha;
							podar = true;
							//cout << "Podando..." << endl;
						}
					}					
					
					if(profundidad == 0){ //Si es el nodo que empezó la recursividad, actualizo los movimientos
						c_piece = color_p;
						id_piece = id_p;
						dice = d;
					}
				}
			}

		}
		
		//cout << "Todos los hijos de este padre (profundidad " << profundidad << ") se han explorado" << endl;
	}
	//nodo terminal
	else{
		//Interpretamos el tablero siempre desde el punto de vista del j0
		//así, j0 quiere maximizar el valor y j1 quiere minimizarlo
		v = heuristic(actual, 0);
	}
	
	return v;
}

/*
* Función que determina los puntos correspondientes en función de la distancia hasta la meta
* Se considera que: si x = 0 (estoy ya en la meta) doy max_ptos.
* Si estoy lo más alejado posible (max_dist) doy 0 puntos
*/
double d(double x, double max_ptos, double max_dist){
	return ( max_ptos*(max_dist - x)) / (max_dist);

}

double HeuristicaGrandMaster2(const Parchis &estado, int jugador){
	int ganador = estado.getWinner();
	int oponente = (jugador + 1) % 2;

	// Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
	if (ganador == jugador)
	{
	  return gana;
	}
	else if (ganador == oponente)
	{
	  return pierde;
	}
	else
	{
		// Colores que juega mi jugador y colores del oponente
		vector<color> my_colors = estado.getPlayerColors(jugador);
		vector<color> op_colors = estado.getPlayerColors(oponente);
		
		Board tablero = estado.getBoard();
	
		double max_distancia = 50;
		int distancia_desde_home = 74;
		
		double max_seguridad = 50;
		double premio_seguridad = max_seguridad / (num_pieces * my_colors.size());
		
		double puntuacion_jugador = 0;
		
		//-----------DISTANCIA------------------
		double distancia_media = 0;
		for(int i = 0; i < my_colors.size(); i++){ //para cada color
			for(int j = 0; j < num_pieces; j++){	//para cada ficha
				distancia_media += estado.distanceToGoal(my_colors[i], j);
			}
		}
		
		distancia_media = distancia_media / (num_pieces*my_colors.size());
		
		//cálculo del valor correspondiente por relación inversamente propocional entre distancia a meta y puntos
		// cerca_meta ---------> max_distancia (estar cerca de la meta te da el máximo puntuaje)
		// distancia_media ----> x (si estoy a distancia_media de la meta, qué puntuación corresponde?)

		puntuacion_jugador += d(distancia_media, max_distancia, distancia_desde_home);
		
		//--------------Amenazas---------------------------
		//Empiezo con la máxima puntuación y penalizo lo que no es bueno
		double puntuacion_seguridad = 0;
		bool amenaza;
		for(int i = 0; i < my_colors.size(); i++){ //para cada uno de mis colores
			for(int j = 0; j < num_pieces; j++){	//para cada pieza
			
				//mi casilla
				Box my_box = tablero.getPiece(my_colors[i], j);
				amenaza = false;
				
				//premio si estoy en una casilla segura, pasillo final o meta
				if(estado.isSafeBox(my_box) or my_box.type == final_queue or my_box.type == goal){
					//puntuacion_seguridad += (my_box.type == goal ? 2*premio_seguridad : premio_seguridad);		
					puntuacion_seguridad += premio_seguridad;
				}
				//si estoy formando una barrera doy algo de recompensa
				else if(estado.isWall(my_box) == my_colors[i]){
					puntuacion_seguridad += premio_seguridad/2;
				}
				//para las piezas que no estén en casa, compruebo amenazas
				else if(my_box.type != home){
					for(int k = 0; k < op_colors.size() and !amenaza; k++){ //para cada color de mi rival
						for(int l = 0; l < num_pieces and !amenaza; l++){	//para cada pieza
						
							//casilla de mi oponente
							Box op_box = tablero.getPiece(op_colors[k], l);
							
							//Veo si soy alcanzable por mi rival
							int distancia = estado.distanceBoxtoBox(op_colors[k], op_box, my_box);
							if(distancia > 0){
								vector<int> dados = estado.getAvailableDices(op_colors[k]);
								
								bool posible = false;
								for(int a = 0; a < dados.size() and !posible; a++){
									if(dados[a] == distancia)
										posible = true;
								}
								
								if(estado.isLegalMove(op_colors[k], op_box, distancia) or posible){
									amenaza = true;								 
								}
							}						
						}
					}
					//Si no tengo amenazas pero no estoy en casilla segura, no doy tanta puntuación
					if(!amenaza){
						puntuacion_seguridad += premio_seguridad/4;
					}					
				} 
			}
		}
		
		puntuacion_jugador += puntuacion_seguridad;
	
		return puntuacion_jugador;
	}

}


double AIPlayer::GrandMaster2(const Parchis &estado, int jugador){
	double puntuacion_jugador, puntuacion_oponente;
	
	puntuacion_jugador = HeuristicaGrandMaster2(estado, jugador);
	puntuacion_oponente = HeuristicaGrandMaster2(estado, ((jugador+1) % 2));
	
	if(puntuacion_jugador == gana or puntuacion_jugador == pierde)
		return puntuacion_jugador;
	
	return puntuacion_jugador - puntuacion_oponente;
}


double AIPlayer::ValoracionTest(const Parchis &estado, int jugador)
{
    // Heurística de prueba proporcionada para validar el funcionamiento del algoritmo de búsqueda.


    int ganador = estado.getWinner();
    int oponente = (jugador + 1) % 2;

    // Si hay un ganador, devuelvo más/menos infinito, según si he ganado yo o el oponente.
    if (ganador == jugador)
    {
        return gana;
    }
    else if (ganador == oponente)
    {
        return pierde;
    }
    else
    {
        // Colores que juega mi jugador y colores del oponente
        vector<color> my_colors = estado.getPlayerColors(jugador);
        vector<color> op_colors = estado.getPlayerColors(oponente);

        // Recorro todas las fichas de mi jugador
        int puntuacion_jugador = 0;
        // Recorro colores de mi jugador.
        for (int i = 0; i < my_colors.size(); i++)
        {
            color c = my_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                // Valoro positivamente que la ficha esté en casilla segura o meta.
                if (estado.isSafePiece(c, j))
                {
                    puntuacion_jugador++;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal)
                {
                    puntuacion_jugador += 5;
                }
            }
        }

        // Recorro todas las fichas del oponente
        int puntuacion_oponente = 0;
        // Recorro colores del oponente.
        for (int i = 0; i < op_colors.size(); i++)
        {
            color c = op_colors[i];
            // Recorro las fichas de ese color.
            for (int j = 0; j < num_pieces; j++)
            {
                if (estado.isSafePiece(c, j))
                {
                    // Valoro negativamente que la ficha esté en casilla segura o meta.
                    puntuacion_oponente++;
                }
                else if (estado.getBoard().getPiece(c, j).type == goal)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

void AIPlayer::think(color & c_piece, int & id_piece, int & dice) const{
    
    switch(id){
    	  case 0: //Heurística definitiva con búsqueda
    	  		cout << "GrandMaster v2 pensando la mejor jugada..." << endl;
    	  		busqueda(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, menosinf, masinf, GrandMaster2);
    	  		break;
    	  case 1: //Heurística ValoraciónTest con búsqueda
    	  		cout << "Valoracion Test" << endl;
    	  		busqueda(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, menosinf, masinf, ValoracionTest);
       	   break;
        case 2:
        		cout << "Seleccionando movimiento aleatorio" << endl;
            thinkAleatorio(c_piece, id_piece, dice);
            break;
        case 3:
       		cout << "Seleccionando movimiento aleatorio+" << endl;
            thinkAleatorioMasInteligente(c_piece, id_piece, dice);            
            break;
        case 4:
        		cout << "Seleccionando movimiento para ficha adelantada" << endl;
            thinkFichaMasAdelantada(c_piece, id_piece, dice);
            break;
        case 5:
        		cout << "Seleccionando mejor movimiento corto plazo" << endl;
        		thinkMejorOpcion(c_piece, id_piece, dice);
        		break;
    }
    
}

