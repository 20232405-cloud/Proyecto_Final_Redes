

def calcular_ping(rtts):
    if not rtts:
        return 0
    return sum(rtts) / len(rtts)

def calcular_jitter(rtts):
    if len(rtts) < 2:
        return 0
    difs = [abs(rtts[i] - rtts[i-1]) for i in range(1, len(rtts))]
    return sum(difs) / len(difs)

def calcular_perdida(enviados, recibidos):
    if enviados == 0:
        return 0
    return (enviados - recibidos) * 100 / enviados

def calcular_qoe_porcentaje(latencia_ms, jitter_ms, perdida_pct):
    """
    Calcula el QoE en porcentaje (0-100%)
    usando latencia promedio, jitter y porcentaje de pérdidas.
    """
    # Fórmula simplificada normalizada
    qoe = 5 - ((latencia_ms / 100.0) + (jitter_ms / 50.0) + (perdida_pct / 10.0))
    
    # Limitar entre 1 y 5
    qoe = max(1, min(5, qoe))
    
    # Convertir a porcentaje (1 equivale a 20%, 5 equivale a 100%)
    qoe_pct = (qoe / 5.0) * 100
    return round(qoe_pct, 2)


def calcular_mos(latencia_ms, jitter_ms, perdida_pct, velocidad_mbps):
    """
    Calcula el MOS en escala 1-5
    considerando latencia, jitter, pérdida y velocidad.
    """
    if latencia_ms < 50 and jitter_ms < 20 and perdida_pct < 1 and velocidad_mbps > 2:
        return 5
    elif latencia_ms < 100 and jitter_ms < 30 and perdida_pct < 2 and velocidad_mbps > 1:
        return 4
    elif latencia_ms < 200 and jitter_ms < 50 and perdida_pct < 5 and velocidad_mbps > 0.5:
        return 3
    elif latencia_ms < 400 and jitter_ms < 100 and perdida_pct < 10:
        return 2
    else:
        return 1