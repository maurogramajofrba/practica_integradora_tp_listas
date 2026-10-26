#ifndef TP_TYPES_H
#define TP_TYPES_H

#define ANCHO_MAPA 200
#define ALTO_MAPA 200

typedef struct OrdenArchivo {
  int x;
  int y;
  unsigned int espera;
  bool soltarGranada1;
  bool soltarGranada2;
  bool ataqueKamikaze;
  bool aterrizaje;
  bool despegue;
  int siguienteX;
  int siguienteY;
} tOrdenArchivo;

typedef struct ChecksAtaque {
  bool hayDespegue;
  bool soltoGranada1;
  bool soltoGranada2;
  bool hayAterrizaje;
  bool hayAtaqueKamikaze;
  int coordenadaDespegueX;
  int coordenadaDespegueY;
  int cantCoordenadas;
  bool coordenadasUsadas[ANCHO_MAPA][ALTO_MAPA];
  int moverseX;
  int moverseY;
} tChecksAtaque;

typedef struct Nodo {
  tOrdenArchivo orden;
  Nodo *siguiente;
} tNodo;

#endif