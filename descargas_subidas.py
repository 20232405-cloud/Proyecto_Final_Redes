
import serial
import re
import json

# Abrir el puerto COM13
ser = serial.Serial('COM13', 115200, timeout=1)

metricas = {}

print("📡 Leyendo métricas del ESP32 en COM13...")

while True:
    linea = ser.readline().decode(errors='ignore').strip()
    if not linea:
        continue
    print(linea)

    # Buscar velocidad de subida
    m = re.search(r"Velocidad de subida:\s*([\d\.]+)", linea)
    if m:
        metricas["velocidad_subida_mbps"] = float(m.group(1))

    # Buscar velocidad de descarga
    m = re.search(r"Velocidad de descarga:\s*([\d\.]+)", linea)
    if m:
        metricas["velocidad_descarga_mbps"] = float(m.group(1))
        break  # ya tenemos ambas métricas, salimos

ser.close()

# Guardar en JSON
with open("metricas.json", "w") as f:
    json.dump(metricas, f, indent=4)

print("\n✅ metricas.json generado:", metricas)