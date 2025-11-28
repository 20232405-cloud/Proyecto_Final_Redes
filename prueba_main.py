
import serial
import re
import json
from indicadores import calcular_ping, calcular_jitter, calcular_perdida, calcular_qoe_porcentaje, calcular_mos


# Configurar el puerto serial
ser = serial.Serial('COM13', 115200)

# Acumuladores globales
total_enviados = 0
total_recibidos = 0

# Métricas por bloque
rtts = []           # Guarda la latencia promedio de cada bloque
bloques_max = 2    # Procesar 5 bloques
bloques_proc = 0

# Estado del bloque actual
latencia_promedio = None
enviados_bloque = None
recibidos_bloque = None

# Utilidades de parseo
re_latencia = re.compile(r'^Latencia promedio:\s*(\d+)\s*ms$')
re_enviados = re.compile(r'^Paquetes enviados\s*:\s*(\d+)$')
re_recibidos = re.compile(r'^Paquetes recibidos\s*:\s*(\d+)$')

print("Leyendo bloques...")

while True:
    linea = ser.readline().decode(errors='ignore').strip()
    if not linea:
        continue
    print(linea)  # log de lo recibido

    # Detectar latencia promedio (RTT del bloque)
    m = re_latencia.match(linea)
    if m:
        latencia_promedio = int(m.group(1))
        continue

    # Detectar enviados del bloque
    m = re_enviados.match(linea)
    if m:
        enviados_bloque = int(m.group(1))
        continue

    # Detectar recibidos del bloque
    m = re_recibidos.match(linea)
    if m:
        recibidos_bloque = int(m.group(1))
        continue

    # Cierre del bloque: cuando tengamos los tres datos
    if (latencia_promedio is not None and
        enviados_bloque is not None and
        recibidos_bloque is not None):

        bloques_proc += 1
        rtts.append(latencia_promedio)
        total_enviados += enviados_bloque
        total_recibidos += recibidos_bloque

        print(f"\n--- Bloque {bloques_proc} ---")
        print(f"Latencia promedio (RTT del bloque): {latencia_promedio} ms")
        print(f"Enviados: {enviados_bloque}")
        print(f"Recibidos: {recibidos_bloque}")
        print(f"Pérdida del bloque: {calcular_perdida(enviados_bloque, recibidos_bloque)} %")
       
        # Si ya tenemos 10 bloques, calcula indicadores y termina
        if bloques_proc == bloques_max:
            print("\nRTTs individuales:")
            for i, valor in enumerate(rtts, start=1):
                print(f"RTT {i}: {valor} ms")

            ping_promedio = calcular_ping(rtts)
            jitter = calcular_jitter(rtts)
            perdida_total = calcular_perdida(total_enviados, total_recibidos)

            print(f"\nPing promedio: {ping_promedio} ms")
            print(f"Jitter: {jitter} ms")
            print(f"Pérdida acumulada: {perdida_total} %")
            print(f"Total enviados: {total_enviados}")
            print(f"Total recibidos: {total_recibidos}")

             # --- QoE y MOS ---
            qoe_pct = calcular_qoe_porcentaje(ping_promedio, jitter, perdida_total)
            mos = calcular_mos(ping_promedio, jitter, perdida_total, velocidad_mbps=2.0)  # ajusta velocidad según tu prueba

            print(f"\nQoE estimado: {qoe_pct}%")
            print(f"MOS estimado: {mos}")

            metricas = {
                "rtts": rtts,
                "ping_promedio": ping_promedio,
                "jitter": jitter,
                "perdida_total": perdida_total,
                "total_enviados": total_enviados,
                "total_recibidos": total_recibidos,
                "qoe": qoe_pct,
                "mos": mos
            }

            with open("metricas.json", "w") as f:
                json.dump(metricas, f, indent=4)

            print("\nmetricas.json generado exitosamente.")

            break

        # Reiniciar estado del bloque (no los acumulados globales)
        latencia_promedio = None
        enviados_bloque = None
        recibidos_bloque = None