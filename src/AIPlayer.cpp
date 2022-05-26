# include "AIPlayer.h"
# include "Parchis.h"

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 4;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta

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

	Parchis siguiente_hijo;
	int decided_dice;
	int decided_piece;
	double v = jugador == 0 ? menosinf : masinf;
	
	
	if(profundidad !=  profundidad_max and !siguiente_hijo.gameOver()){ //Exploro el árbol de búsqueda mientras no llegue a un nodo terminal		
		
		while(!(siguiente_hijo == actual)){ //Genero todos los hijos del nodo
			double v_aux;
			
			//Genero el siguiente hijo
			siguiente_hijo = actual.generateNextMove(c_piece, id_piece, dice); 
			
			//Actualizo al jugador
			jugador = siguiente_hijo.getCurrentPlayerId();
			
			//Inicio la busqueda en el hijo y obtengo la heurística asociada
			v_aux = busqueda(siguiente_hijo, jugador, ++profundidad, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);
				
			//Actualizo la heurísitca del nodo padre
			if((jugador == 0 and v_aux > v) or (jugador == 1 and v_aux < v)){
				v = v_aux;
				decided_dice = dice;
				decided_piece = id_piece;
			}
		}
		
	}
	else{ //Si el nodo es terminal, calculo la heurística asociada al tablero
		v = heuristic(siguiente_hijo, jugador);
	}
	
	//Si es el nodo padre, actualizo los valores del movimiento seleccionado
	if(profundidad == 0){
		dice = decided_dice;
		id_piece = decided_piece;
	}
	
	//Devuelvo la heurística del nodo
	return v;
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
                else if (estado.getBoard().getPiece(c, j).type == home)
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
                else if (estado.getBoard().getPiece(c, j).type == home)
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
    	  
    	  break;
    	  case 1: //Heurística ValoraciónTest con búsqueda
    	  		busqueda(*actual, actual->getCurrentPlayerId(), 0, PROFUNDIDAD_MINIMAX, c_piece, id_piece, dice, 0, 0, ValoracionTest);
        break;
        case 2:
            thinkAleatorio(c_piece, id_piece, dice);
            break;
        case 3:
            thinkAleatorioMasInteligente(c_piece, id_piece, dice);            
            break;
        case 4:
            thinkFichaMasAdelantada(c_piece, id_piece, dice);
            break;
        case 5:
        		thinkMejorOpcion(c_piece, id_piece, dice);
        		break;
    }
    
}

