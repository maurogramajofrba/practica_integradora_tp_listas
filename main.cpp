#include "tp_types.h"
#include <filesystem>
#include <iostream>

using namespace std;

void ImprimirArchivo();
void ImprimirOrden(tOrdenArchivo orden);
void ImprimirLista(tNodo *lista);
void ImprimirArchivo(string rutaCompleta);
void CargarEnLista(tNodo *&listaAtaque, string &rutaCompleta);
void InsertarNodo(tNodo *&lista, tOrdenArchivo ordenNueva);
void IngresarOrden(tOrdenArchivo &orden);
void ModificarOrdenArchivo(string rutaCompleta);
void ModificarOrdenLista(tNodo *lista);
void BorrarPrimerNodo(tNodo *&lista);
void BorrarLista(tNodo *&lista);
void GenerarReporteHTML(tNodo *lista, string rutaCompleta);

int main() {
  tNodo *listaAtaque = nullptr;
  string rutaCompleta = "";

  CargarEnLista(listaAtaque, rutaCompleta);
  ImprimirLista(listaAtaque);
  // ModificarOrdenArchivo(rutaCompleta);
  // ImprimirArchivo(rutaCompleta);
  // ImprimirLista(listaAtaque);
  // ModificarOrdenLista(listaAtaque);
  // ImprimirArchivo(rutaCompleta);
  // ImprimirLista(listaAtaque);
  GenerarReporteHTML(listaAtaque, rutaCompleta);
  BorrarLista(listaAtaque);
  return 0;
}

void IngresarOrden(tOrdenArchivo &orden) {
  cout << "Coordenadas X Y: ";
  cin >> orden.x >> orden.y;

  cout << "Tiempo de espera: ";
  cin >> orden.espera;

  cout << "Despega? (0 = NO, 1 = SI): ";
  cin >> orden.despegue;

  cout << "Aterriza? (0 = NO, 1 = SI): ";
  cin >> orden.aterrizaje;

  cout << "Suelta granada 1? (0 = NO, 1 = SI): ";
  cin >> orden.soltarGranada1;

  cout << "Suelta granada 2? (0 = NO, 1 = SI): ";
  cin >> orden.soltarGranada2;

  cout << "Ataque kamikaze? (0 = NO, 1 = SI): ";
  cin >> orden.ataqueKamikaze;

  if (!orden.aterrizaje && !orden.ataqueKamikaze) {
    cout << "Siguiente coordenada X Y: ";
    cin >> orden.siguienteX >> orden.siguienteY;
  }
}

void ModificarOrdenArchivo(string rutaCompleta) {
  FILE *archivoAtaque;

  if (strlen(rutaCompleta.c_str()) < 5) {
    cout << "No hay una ruta de archivo cargada!!" << endl;
    cout << "Longitud de ruta completa: " << strlen(rutaCompleta.c_str()) << endl;
    return;
  }

  archivoAtaque = fopen(rutaCompleta.c_str(), "rb+");
  if (archivoAtaque == NULL) {
    cout << "No se pudo abrir el archivo: " << rutaCompleta << endl;
    return;
  }

  int posicion = 0;
  long tamanioEstructura = sizeof(tOrdenArchivo);
  cout << "Que orden desea corregir?" << endl;
  cin >> posicion;

  int output;
  if (fseek(archivoAtaque, tamanioEstructura * (posicion - 1), SEEK_SET)) {
    cout << "Fallo ajustar indice del archivo!" << endl;
    return;
  }

  tOrdenArchivo ordenLeida;
  if (fread(&ordenLeida, tamanioEstructura, 1, archivoAtaque)) {
    cout << "Estado actual de la oden:" << endl;
    ImprimirOrden(ordenLeida);
  }

  if (fseek(archivoAtaque, -tamanioEstructura, SEEK_CUR)) {
    cout << "Fallo ajustar indice del archivo!" << endl;
    return;
  }

  IngresarOrden(ordenLeida);

  if (!fwrite(&ordenLeida, sizeof(tOrdenArchivo), 1, archivoAtaque)) {
    cout << "Fallo actualizar la orden!!" << endl;
    return;
  }

  cout << "Orden actualizada con exito!!!" << endl;

  fclose(archivoAtaque);
}

void ModificarOrdenLista(tNodo *lista) {
  int x, y;
  tNodo *nodoModificado = nullptr;

  if (lista == nullptr) {
    cout << "No hay un archivo cargado en memoria!!!" << endl;
    return;
  }

  cout << "Que orden en memoria desea corregir? ingrese x y" << endl;
  cin >> x >> y;

  nodoModificado = lista;
  bool encontrado = false;

  while (nodoModificado != nullptr) {
    if (nodoModificado->orden.x == x && nodoModificado->orden.y == y) {
      encontrado = true;
      break;
    }

    nodoModificado = nodoModificado->siguiente;
  }

  if (!encontrado) {
    cout << "Orden no encontrada!!" << endl;
    return;
  }

  IngresarOrden(nodoModificado->orden);
}

void CargarEnLista(tNodo *&listaAtaque, string &rutaCompleta) {
  if (listaAtaque != nullptr) {
    BorrarLista(listaAtaque);
  }

  FILE *archivoAtaque;

  cout << "Decime la ruta completa del archivo de ataque:" << endl;
  cout << "> ";
  getline(cin, rutaCompleta);

  archivoAtaque = fopen(rutaCompleta.c_str(), "rb");

  if (archivoAtaque == NULL) {
    cout << "No se pudo abrir el archivo: " << rutaCompleta << endl;
    return;
  }

  cout << "Archivo abierto correctamente." << endl;

  tOrdenArchivo ordenLeida;

  while (fread(&ordenLeida, sizeof(tOrdenArchivo), 1, archivoAtaque)) {
    InsertarNodo(listaAtaque, ordenLeida);
  }

  fclose(archivoAtaque);
}

void ImprimirOrden(tOrdenArchivo orden) {
  cout << "[" << orden.x << " " << orden.y << "]: Despega: ";
  if (orden.despegue)
    cout << "SI";
  else
    cout << "NO";
  cout << " | Aterriza: ";
  if (orden.aterrizaje)
    cout << "SI";
  else
    cout << "NO";
  cout << " | Suelta granada 1: ";
  if (orden.soltarGranada1)
    cout << "SI";
  else
    cout << "NO";
  cout << " | Suelta granada 2: ";
  if (orden.soltarGranada2)
    cout << "SI";
  else
    cout << "NO";
  cout << " | Ataque kamikaze: ";
  if (orden.ataqueKamikaze)
    cout << "SI";
  else
    cout << "NO ";

  cout << " | Espera: ";
  if (orden.espera) {
    cout << orden.espera << " s";
  } else {
    cout << "NO";
  }

  if (!orden.ataqueKamikaze && !orden.aterrizaje)
    cout << " | Se mueve a: [" << orden.siguienteX << " " << orden.siguienteY << "]";

  cout << endl;
}

void ImprimirLista(tNodo *lista) {
  if (lista == nullptr) {
    cout << "No hay ataque cargado en memoria!!" << endl;
    return;
  }

  cout << "Ataque cargado en memoria:" << endl;
  int contador = 1;
  while (lista != nullptr) {
    cout << "Orden " << contador << " -> ";
    ImprimirOrden(lista->orden);
    lista = lista->siguiente;
    contador++;
  }
  return;
}

void ImprimirArchivo(string rutaCompleta) {
  if (strlen(rutaCompleta.c_str()) < 5) {
    cout << "No hay ruta de archivo cargado!!!" << endl;
    return;
  }

  FILE *archivoAtaque = fopen(rutaCompleta.c_str(), "rb");

  if (!archivoAtaque) {
    cout << "No se pudo abrir archivo!!" << endl;
    return;
  }

  cout << "Ataque en archivo: " << rutaCompleta << endl;
  int contador = 1;
  tOrdenArchivo ordenLeida;
  while (fread(&ordenLeida, sizeof(tOrdenArchivo), 1, archivoAtaque)) {
    cout << "Orden " << contador << " -> ";
    ImprimirOrden(ordenLeida);
    contador++;
  }
  fclose(archivoAtaque);
  return;
}

void InsertarNodo(tNodo *&lista, tOrdenArchivo ordenNueva) {
  tNodo *aux;

  if (lista == nullptr) {
    lista = new tNodo{ordenNueva};
  } else {
    aux = lista;
    while (aux->siguiente != nullptr)
      aux = aux->siguiente;

    aux->siguiente = new tNodo{ordenNueva};
  }
  return;
}

void BorrarPrimerNodo(tNodo *&lista) {
  tNodo *aux;

  if (lista != nullptr) {
    aux = lista;
    lista = lista->siguiente;
    delete aux;
  }
  return;
}

void BorrarLista(tNodo *&lista) {
  while (lista != nullptr) {
    BorrarPrimerNodo(lista);
  }
  return;
}

void GenerarReporteHTML(tNodo *lista, string rutaCompleta) {
  if (lista == nullptr) {
    cout << "No hay ataque cargado en memoria!!!" << endl;
    return;
  }

  filesystem::path ruta(rutaCompleta);
  ruta.replace_extension(".html");

  FILE *reporteHTML = fopen(ruta.string().c_str(), "w");

  if (!reporteHTML) {
    cout << "No se pudo crear reporte html!!!" << endl;
  }

  fprintf(reporteHTML, "<!DOCTYPE html>\n");
  fprintf(reporteHTML, "<html>\n");
  fprintf(reporteHTML, "<head>\n");
  fprintf(reporteHTML, "<title>\n");
  fprintf(reporteHTML, "Reporte archivo %s\n", ruta.filename().string().c_str());
  fprintf(reporteHTML, "</title>\n");
  fprintf(reporteHTML, "</head>\n");
  fprintf(reporteHTML, "<body>\n");
  fprintf(reporteHTML, "<h1>\n");
  fprintf(reporteHTML, "Reporte archivo %s\n", ruta.filename().string().c_str());
  fprintf(reporteHTML, "</h1>\n");
  fprintf(reporteHTML, "<table>\n");
  fprintf(reporteHTML,
          "<thead><tr><th></th><th>Coordenada</th><th>Despega</th><th>Aterriza</th><th>Suelta "
          "granada 1</th><th>Suelta granada 2</th><th>Ataque kamikaze</th><th>Espera</th><th>Se "
          "mueve a</th></tr></thead>\n");
  fprintf(reporteHTML, "<tbody>\n");

  int contador = 1;
  while (lista != nullptr) {
    fprintf(reporteHTML, "<tr><th>%d</th>", contador);
    fprintf(reporteHTML, "<td>%d %d</td>", lista->orden.x, lista->orden.y);

    if (lista->orden.despegue)
      fprintf(reporteHTML, "<td>SI</td>");
    else
      fprintf(reporteHTML, "<td>NO</td>");

    if (lista->orden.aterrizaje)
      fprintf(reporteHTML, "<td>SI</td>");
    else
      fprintf(reporteHTML, "<td>NO</td>");

    if (lista->orden.soltarGranada1)
      fprintf(reporteHTML, "<td>SI</td>");
    else
      fprintf(reporteHTML, "<td>NO</td>");

    if (lista->orden.soltarGranada2)
      fprintf(reporteHTML, "<td>SI</td>");
    else
      fprintf(reporteHTML, "<td>NO</td>");

    if (lista->orden.ataqueKamikaze)
      fprintf(reporteHTML, "<td>SI</td>");
    else
      fprintf(reporteHTML, "<td>NO</td>");

    if (lista->orden.espera) {
      fprintf(reporteHTML, "<td>%d s.</td>", lista->orden.espera);
    } else {
      fprintf(reporteHTML, "<td>-</td>");
    }

    if (!lista->orden.ataqueKamikaze && !lista->orden.aterrizaje)
      fprintf(reporteHTML, "<td>%d %d</td>", lista->orden.x, lista->orden.y);
    else
      fprintf(reporteHTML, "<td>-</td>");

    fprintf(reporteHTML, "</tr>\n");
    lista = lista->siguiente;
    contador++;
  }
  fprintf(reporteHTML, "</tbody>\n");
  fprintf(reporteHTML, "</table>\n");

  fprintf(reporteHTML, "</body>\n");
  fprintf(reporteHTML, "</html>\n");

  fclose(reporteHTML);
  return;
}