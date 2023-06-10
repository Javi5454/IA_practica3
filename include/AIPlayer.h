#ifndef __AI_PLAYER_H__
#define __AI_PLAYER_H__

#include "Attributes.h"
#include "Player.h"

class AIPlayer : public Player
{
protected:
    // Id identificativo del jugador
    const int id;

public:
    /**
     * @brief Constructor de un objeto AIPlayer
     *
     * @param name Nombre del jugador
     */
    inline AIPlayer(const string &name) : Player(name), id(1){};

    /**
     * @brief Constructor de un objeto AIPlayer
     *
     * @param name Nombre  del jugador
     * @param id Id del jugador
     */
    inline AIPlayer(const string &name, const int id) : Player(name), id(id){};

    /**
     * @brief Función que percibe el el parchís y al jugador actual.
     * Asigna el tablero en actual y el id del jugador.
     *
     * @param p Instancia Parchis que se quiere percibir
     */
    inline virtual void perceive(Parchis &p) { Player::perceive(p); }

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
    virtual void think(color &c_piece, int &id_piece, int &dice) const;

    /**
     * @brief Método que determina si el player es inteligente (decide el mejor movimiento)
     * o no. True para AIPlayer.
     *
     * @return true
     * @return false
     */
    inline virtual bool canThink() const { return true; }

    /**
     * @brief Heurística de prueba para validar el algoritmo de búsqueda.
     *
     * @param estado Instancia de Parchis con el estado actual de la partida.
     * @param jugador Id del jugador actual (0 o 1)
     * @return double
     */
    static double ValoracionTest(const Parchis &estado, int jugador);

    /**
     * @brief Propuesta de declaración de la función poda alfa-beta.
     * La propuesta es solo sugerencia, los parámetros de la declaración podrían variar.
     */
    double Poda_AlfaBeta(const Parchis &actual, int jugador, int profundidad, int profundidad_max, color &c_piece, int &id_piece, int &dice, double alpha, double beta, double (*heuristic)(const Parchis &, int)) const;

    // Tutorial
    void thinkAleatorio(color &c_piece, int &id_piece, int &dice) const;
    void thinkAleatorioMasInteligente(color &c_piece, int &id_piece, int &dice) const;
    void thinkFichaMasAdelantada(color &c_piece, int &id_piece, int &dice) const;
    void thinkMejorOpcion(color &c_piece, int &id_piece, int &dice) const;

    /**
     * @brief Heurística realizada para vencer a los ninjas
     * @pre 0 <= jugador <= 1
     * @param estado Instancia de parchís con un estado de partida
     * @param jugador Id del jugador actual
     * @return double Valoración de la heurística
     */

    static double BestHeuristic(const Parchis &estado, int jugador);

    /**
     * @brief Calculador de una puntuación de un jugador en un determinado momento de la partida
     * @pre 0 <= jugador <= 1
     * @param estado Instancia de parchís con un estado de partida
     * @param jugador Id del jugador actual
     * @return double Puntuacion de un jugador en un determinado estado
    */

   //Progresivas versiones de scoring
   static double scoringV1(const Parchis &estado, int jugador); //Ninja 1
   static double scoringV2(const Parchis &estado, int jugador); //Ninja 1 y 2
   static double scoringV3(const Parchis &estado, int jugador); //Ninja 1, 2 y 3
};
#endif