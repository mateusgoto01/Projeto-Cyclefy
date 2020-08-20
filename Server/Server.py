from raspi_lora import LoRa, ModemConfig
from firebase import firebase
def on_recv(payload): # Define a recepção dos dados
    if payload.heard_from == 2: # Se a lixeira 1 comunicar
        print("From:", payload.header_from) # Confere que quem enviou é a lixeria 1
        print("Received:", payload.message) # Printa a mensagem da lixeira
        data_list = [float(data) for data in payload.message.split('$')]
        firebase.put("Peso: ", datalist[0], "Kg")
        firebase.put("Altura: ", datalist[1], "cm")



    if payload.heard_from = 3: # Se a lixeira 2 comunicar
        print("From:", payload.header_from)
        print("Received:", payload.message)
        data_list = [float(data) for data in payload.message.split('$')]
        firebase.put("Peso: ", datalist[0], "Kg")
        firebase.put("Altura: ", datalist[1], "cm")





lora = LoRa(0, 17, 1, modem_config=ModemConfig.Bw125Cr45Sf128, tx_power=14, acks=True)
lora.on_recv = on_recv