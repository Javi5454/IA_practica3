#include "AIPlayer.h"
#include "Parchis.h"

const double masinf = 9999999999.0, menosinf = -9999999999.0;
const double gana = masinf - 1, pierde = menosinf + 1;
const int num_pieces = 3;
const int PROFUNDIDAD_MINIMAX = 4;  // Umbral maximo de profundidad para el metodo MiniMax
const int PROFUNDIDAD_ALFABETA = 6; // Umbral maximo de profundidad para la poda Alfa_Beta

bool AIPlayer::move()
{
    cout << "Realizo un movimiento automatico" << endl;

    color c_piece;
    int id_piece;
    int dice;
    think(c_piece, id_piece, dice);

    cout << "Movimiento elegido: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    actual->movePiece(c_piece, id_piece, dice);
    return true;
}

void AIPlayer::think(color &c_piece, int &id_piece, int &dice) const
{

    // El siguiente código se proporciona como sugerencia para iniciar la implementación del agente.

    double valor;                           // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha = menosinf, beta = masinf; // Cotas iniciales de la poda AlfaBeta
    // Llamada a la función para la poda (los parámetros son solo una sugerencia, se pueden modificar).
    // valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;

    // ----------------------------------------------------------------- //

    // Si quiero poder manejar varias heurísticas, puedo usar la variable id del agente para usar una u otra.
    switch (id)
    {
    case 0:
        valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, BestHeuristic);
        break;
    case 1:
        thinkAleatorioMasInteligente(c_piece, id_piece, dice);
        break;
    case 2:
        thinkFichaMasAdelantada(c_piece, id_piece, dice);
        break;
    case 3:
        thinkMejorOpcion(c_piece, id_piece, dice);
    case 4:
        valor = Poda_AlfaBeta(*actual, jugador, 0, PROFUNDIDAD_ALFABETA, c_piece, id_piece, dice, alpha, beta, ValoracionTest);
        break;
    }
    cout << "Valor MiniMax: " << valor << "  Accion: " << str(c_piece) << " " << id_piece << " " << dice << endl;
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
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
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
                else if (estado.getBoard().getPiece(c, j).get_box().type == goal)
                {
                    puntuacion_oponente += 5;
                }
            }
        }

        // Devuelvo la puntuación de mi jugador menos la puntuación del oponente.
        return puntuacion_jugador - puntuacion_oponente;
    }
}

void AIPlayer::thinkAleatorio(color &c_piece, int &id_piece, int &dice) const
{
    // El numero de mi jugador actual
    int player = actual->getCurrentPlayerId();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices;

    // Vector que almacenara los ids de las fichas que se pueedn mvoer para el dado elegido
    vector<tuple<color, int>> current_pieces;

    // Se obtiene el vector de dados que se pueden usar para el movmiento
    current_dices = actual->getAvailableNormalDices(player);

    // Elijo un dado de forma aleatoria
    dice = current_dices[rand() % current_dices.size()];

    // Se obtiene el vector de fichas que se pueden mover para el dado elegido
    current_pieces = actual->getAvailablePieces(player, dice);

    // Si tengo fichas para el dado elegido muevo una al azar
    if (current_pieces.size() > 0)
    {
        int random_id = rand() % current_pieces.size();
        id_piece = get<1>(current_pieces[random_id]);
        c_piece = get<0>(current_pieces[random_id]);
    }
    else
    {
        // Si no tengo fichas para el dado elegido, paso turno
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor();
    }
}

void AIPlayer::thinkAleatorioMasInteligente(color &c_piece, int &id_piece, int &dice) const
{
    // El numero de mi jugador actual
    int player = actual->getCurrentPlayerId();

    // Vector que almacenará los dados que se pueden usar para el movimiento
    vector<int> current_dices_con_especiales;

    // Vector que almacenará los ids de las fichas que se pueden mover para el dado elegido
    vector<tuple<color, int>> current_pieces;

    // Obtengo el vector de dados que puedo usar para el movimiento.
    // En este caso elijo todos, tanto normales como especiales.
    // Importante: puedo acceder a solo dados normales o especiales por separado,
    // o a todos a la vez:
    // - actual->getAvailableNormalDices(player) -> solo dados normales
    // - actual->getAvailableSpecialDices(player) -> solo dados especiales
    // - actual->getAllAvailableDices(player) -> todos los dados
    // Importante 2: los "available" me dan los dados que puedo usar en el turno actual.
    // Por ejemplo, si me tengo que contar 10 o 20 solo me saldrán los dados 10 y 20.
    // Puedo saber qué más dados tengo, aunque no los pueda usar en este turno, con:
    // - actual->getNormalDices(player) -> todos los dados normales
    // - actual->getSpecialDices(player) -> todos los dados especiales
    // - actual->getAllDices(player) -> todos los dados
    current_dices_con_especiales = actual->getAllAvailableDices(player);

    // En vez de elegir un dado al azar, miro primero cuales tiene fichas que se pueden mover
    vector<int> current_dices_que_pueden_mover_ficha;

    for (int i = 0; i < current_dices_con_especiales.size(); i++)
    {
        // Se obtiene el vector de fichas que se pueden mover para el dado elegido
        current_pieces = actual->getAvailablePieces(player, current_dices_con_especiales[i]);

        // Si se pueden mover fichas para el dado actual, lo añado al vector de dados que pueden mvoer fichas
        if (current_pieces.size() > 0)
        {
            current_dices_que_pueden_mover_ficha.push_back(current_dices_con_especiales[i]);
        }
    }

    // Si no tengo ningun dado que pueda mover fichas, paso turno con un dado al azar
    if (current_dices_que_pueden_mover_ficha.size() == 0)
    {
        dice = current_dices_con_especiales[rand() % current_dices_con_especiales.size()];

        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor();
    }
    else
    {
        // En caso contrario, elijo un dado de forma aleatoria de enttre los que pueden mover ficha
        dice = current_dices_que_pueden_mover_ficha[rand() % current_dices_que_pueden_mover_ficha.size()];

        // Se obtiene el vector de fichas que se pueden mvoer para el dado elegido
        current_pieces = actual->getAvailablePieces(player, dice);

        // Muevo una ficha al azar de entre las que se pueden mover
        int random_id = rand() % current_pieces.size();

        id_piece = get<1>(current_pieces[random_id]);

        c_piece = get<0>(current_pieces[random_id]);
    }
}

void AIPlayer::thinkFichaMasAdelantada(color &c_piece, int &id_piece, int &dice) const
{
    // Elijo el dado haciendo lo mismo que el jugador anterior
    thinkAleatorioMasInteligente(c_piece, id_piece, dice);

    // Tras llamar a esta funcion, ya tengo en dice el numero de dado que quiero usar
    // Ahora, en vez de mover una ficha al azar, voy aplicar el dado a la que esté mas adelantada
    int player = actual->getCurrentPlayerId();
    vector<tuple<color, int>> current_pieces = actual->getAvailablePieces(player, dice);

    int id_ficha_mas_adelantada = -1;
    color col_ficha_mas_adelantada = none;
    int min_distancia_meta = 9999;

    for (int i = 0; i < current_pieces.size(); i++)
    {
        color col = get<0>(current_pieces[i]);
        int id = get<1>(current_pieces[i]);
        int distancia_meta = actual->distanceToGoal(col, id);

        if (distancia_meta < min_distancia_meta)
        {
            min_distancia_meta = distancia_meta;
            id_ficha_mas_adelantada = id;
            col_ficha_mas_adelantada = col;
        }
    }

    // Si no he encontrado ninguna ficha, paso turno
    if (id_ficha_mas_adelantada == -1)
    {
        id_piece = SKIP_TURN;
        c_piece = actual->getCurrentColor();
    }
    else
    {
        // En caso contrario, movere la ficha mas adelantada
        id_piece = id_ficha_mas_adelantada;
        c_piece = col_ficha_mas_adelantada;
    }
}

void AIPlayer::thinkMejorOpcion(color &c_piece, int &id_piece, int &dice) const
{
    // Vamos a mirar todos los posibles movimientos del jugador actual accediendo a los hijos del estado actual

    // Para ello, vamos a iterar sobre los hijos con la funcion de Parchis getChildren()
    // Esta funcion devuelve un objeto de la clase ParchisBros, que es una estructura iterable
    // sobre la que se pueden recorrer todos los hijos del estado sobre el que se llama
    ParchisBros hijos = actual->getChildren();

    bool me_quedo_con_esta_accion = false;

    // La clase ParchisBros viene con un iterador muy útil y sencillo de usar
    // Al hacer begin() accedemos al primer hijo de la rama
    // y cada vez que hagamos ++it saltaremos al siguiente hijo
    // Comparando con el iterador end() podemos consultar cuando hemos terminado de visitar los hijos
    for (ParchisBros::Iterator it = hijos.begin(); it != hijos.end(); ++it)
    {
        Parchis siguiente_hijo = *it;

        if (siguiente_hijo.isEatingMove() || siguiente_hijo.isGoalMove() || (siguiente_hijo.gameOver() && siguiente_hijo.getWinner() == this->jugador))
        {
            me_quedo_con_esta_accion = true;
            c_piece = it.getMovedColor();
            id_piece = it.getMovedPieceId();
            dice = it.getMovedDiceValue();
        }
    }

    // Si he encontrado una accion que me interesa, la guardo en las variables pasadas por referencia

    // Si no, muevo la ficha más adelantada como antes
    if (!me_quedo_con_esta_accion)
    {
        thinkFichaMasAdelantada(c_piece, id_piece, dice);
    }
}

double AIPlayer::Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta,
                               double (*heuristic)(const Parchis &, int)) const
{
    // Si el juego ha terminado o hemos llegado hasta la profundiadd maxima
    if (actual.gameOver() || profundidad == profundidad_max)
    {
        // Devolvemos mi heuristica en este estado
        return heuristic(actual, jugador);
    }
    else
    {
        ParchisBros hijos = actual.getChildren();

        // Generemos los nuevos hijos
        for (ParchisBros::Iterator it = hijos.begin(); it != hijos.end(); ++it)
        {
            if (this->jugador != actual.getCurrentPlayerId())
            { // Nodos MIN
                double valor_actual = Poda_AlfaBeta(*it, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);

                if (valor_actual < beta)
                {
                    beta = valor_actual;
                }

                if (alpha >= beta)
                {
                    break;
                }
            }
            else
            { // Nodos MAX
                double valor_actual = Poda_AlfaBeta(*it, jugador, profundidad + 1, profundidad_max, c_piece, id_piece, dice, alpha, beta, heuristic);

                if (alpha < valor_actual)
                {
                    alpha = valor_actual;

                    if (profundidad == 0)
                    {
                        c_piece = it.getMovedColor();
                        id_piece = it.getMovedPieceId();
                        dice = it.getMovedDiceValue();
                    }
                }

                if (alpha >= beta)
                {
                    break;
                }
            }
        }

        // Devolvemos alpha o beta segun del player
        if (jugador == actual.getCurrentPlayerId())
        {
            return alpha;
        }
        else
        {
            return beta;
        }
    }
}

//-------------
// HEURÍSTICAS
//-------------

double AIPlayer::BestHeuristic(const Parchis &estado, int jugador)
{
    int winner = estado.getWinner();
    int opponent = (jugador + 1) % 2;

    if (jugador == winner)
    { // Si hemos ganado
        return gana;
    }
    else if (opponent == winner)
    { // Si hemos perdido
        return pierde;
    }
    else
    { // En cualquier otro caso
        int winner_score = 0;
        int opponent_score = 0;

        winner_score = scoringV3(estado, jugador);
        opponent_score = scoringV3(estado, opponent);

        return winner_score - opponent_score;
    }
}

double AIPlayer::scoring(const Parchis &estado, int jugador)
{
    double puntuacion = 0;
    vector<color> colores = estado.getPlayerColors(jugador);

    // Recorro mis colores y las fichas de cada color
    for (int i = 0; i < colores.size(); i++)
    {
        color c = colores[i];
        // Recorro las fichas de cada color
        int fichas_en_meta = estado.piecesAtGoal(c);
        puntuacion += 90 * fichas_en_meta; // Damos 100 puntos a cada ficha en la casilla de llegada

        if (estado.piecesAtHome(c) > 0)
        {
            puntuacion -= 333 * estado.piecesAtHome(c);
        }

        for (int j = 0; j < num_pieces; j++)
        {
            int distancia_a_meta = estado.distanceToGoal(c, j);
            puntuacion -= distancia_a_meta * 2;

            if (estado.isSafePiece(c, j))
            {
                puntuacion += 300;
            }
        }
    }

    pair<color, int> eaten_piece = estado.eatenPiece();

    color c = eaten_piece.first;

    if (c == colores[0] || c == colores[1])
    {
        puntuacion -= 500;
    }
    else if (c != none)
    {
        puntuacion += 300;
    }

    const vector<pair<color, int>> destroyed_pieces = estado.piecesDestroyedLastMove();

    for (int i = 0; i < destroyed_pieces.size(); i++)
    {
        pair<color, int> current_pair = destroyed_pieces[i];

        if (current_pair.first == colores[0] || current_pair.first == colores[1])
        {
            puntuacion -= 300;
        }
        else
        {
            puntuacion += 400;
        }
    }

    if (estado.gameOver() and estado.getWinner() == jugador)
    {
        puntuacion += gana;
    }

    return puntuacion;
}

double AIPlayer::scoringV1(const Parchis &estado, int jugador)
{
    double puntuacion = 0;
    vector<color> colores = estado.getPlayerColors(jugador);

    // Recorro mis colores
    for (int i = 0; i < colores.size(); i++)
    {
        // Recorro mis fichas
        for (int j = 0; j < num_pieces; j++)
        {
            puntuacion -= estado.distanceToGoal(colores[i], j);
        }
    }

    return puntuacion;
}

double AIPlayer::scoringV2(const Parchis &estado, int jugador)
{
    double puntuacion = 0;
    vector<color> colores = estado.getPlayerColors(jugador);

    // Recorro mis colores
    for (int i = 0; i < colores.size(); i++)
    {
        // Recorro mis fichas
        for (int j = 0; j < num_pieces; j++)
        {
            puntuacion -= estado.distanceToGoal(colores[i], j) * 2;

            // Contamos las piezas que estan en un lugar seguro
            if (estado.isSafePiece(colores[i], j))
            {
                puntuacion += 100; // 100 puntos por ficha
            }
        }

        // Contamos las fichas en casa
        puntuacion -= estado.piecesAtHome(colores[i]) * 200;

        // Contamos las fichas en la meta
        puntuacion += estado.piecesAtGoal(colores[i]) * 90;
    }

    // Contamos ahora los dados especiales
    vector<int> dadosEspeciales = estado.getAvailableSpecialDices(jugador);

    puntuacion += dadosEspeciales.size() * 50; // 50 puntos cada dado

    return puntuacion;
}

double AIPlayer::scoringV3(const Parchis &estado, int jugador)
{
    double puntuacion = 0;
    vector<color> colores = estado.getPlayerColors(jugador);

    // Recorro mis colores
    for (int i = 0; i < colores.size(); i++)
    {
        // Recorro mis fichas
        for (int j = 0; j < num_pieces; j++)
        {
            puntuacion -= estado.distanceToGoal(colores[i], j) * 2;

            // Contamos las piezas que estan en un lugar seguro
            if (estado.isSafePiece(colores[i], j))
            {
                puntuacion += 100; // 100 puntos por ficha
            }
        }

        // Contamos las fichas en casa
        puntuacion -= estado.piecesAtHome(colores[i]) * 200;

        // Contamos las fichas en la meta
        puntuacion += estado.piecesAtGoal(colores[i]) * 90;
    }

    // Contamos ahora los dados especiales
    vector<int> dadosEspeciales = estado.getAvailableSpecialDices(jugador);

    puntuacion += dadosEspeciales.size() * 50; // 50 puntos cada dado

    // TODO: Barrera y barrera con seguro

    // Comprobamos si me han comido o si como yo
    pair<color, int> eaten_piece = estado.eatenPiece();
    color eaten_color = eaten_piece.first;

    if (eaten_color == colores[0] || eaten_color == colores[1])
    {
        puntuacion -= 500; // Me han comido
    }
    else
    {
        puntuacion += 600; // He comido
    }

    //Si ganamos
    if(estado.gameOver() && estado.getWinner() == jugador){
        puntuacion += gana;
    }

    return puntuacion;
}