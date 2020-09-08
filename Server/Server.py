from raspi_lora import LoRa, ModemConfig
import pyrebase


config = {
  "apiKey": "AIzaSyCWH1RjnADOMq_vjOJmelWc0uYOPZ8AX_0",
  "authDomain": "cyclefy-5f768.firebaseapp.com",
  "databaseURL": "https://cyclefy-5f768.firebaseio.com",
  "storageBucket": "cyclefy-5f768.appspot.com"
}

firebase = pyrebase.initialize_app(config)
db = firebase.database()


def on_recv(payload):
    if payload.header_from == 2:
        print("From: ", payload.header_from)
        print("Received: ", payload.message)
        dados = bytes(payload.message).decode("utf-8")
        print(dados)
        data_list = [str(data) for data in dados.split('$')]
        print("Peso: ", data_list[1])
        print("Altura: ", data_list[2])
        
        db.child("Condomínio 1").child("lixeira 1").child("Peso:").set(data_list[1])
        db.child("Condomínio 1").child("lixeira 1").child("Altura:").set(data_list[2])
    if payload.header_from == 3:
        print("From: ", payload.header_from)
        print("Received: ", payload.message)
        dados = bytes(payload.message).decode("utf-8")
        print(dados)
        data_list = [str(data) for data in dados.split('$')]
        print("Peso: ", data_list[1])
        print("Altura: ", data_list[2])
        db.child("Condomínio 1").child("lixeira 2").child("Peso:").set(data_list[1])
        db.child("Condomínio 1").child("lixeira 2").child("Altura:").set(data_list[2])



lora = LoRa(0, 17, 1, freq=868, modem_config=ModemConfig.Bw31_25Cr48Sf512, tx_power=14, acks=True)

print("iniciando...")
lora.on_recv = on_recv
lora.set_mode_rx()
print("Finalizado")