from machine import UART, Pin
import time
phoneme_chars = [
'A', 'AA', 'AE', 'AH', 'AW', 'AX', 'AY',
'B', 'CH', 'D', 'DH', 'DX', 'E', 'EH',
'EI', 'ER', 'EW', 'EY', 'F', 'G', 'H',
'I', 'IH', 'IX', 'IY', 'J', 'K', 'KX',
'L', 'M', 'N', 'NG', 'NY', 'O', 'OW',
'OY', 'P', 'PX', 'R', 'RR', 'S', 'SH',
'T', 'TH', 'TX', 'U', 'UH', 'UW', 'V',
'W', 'WH', 'Y', 'YY', 'Z', 'ZH'
]
phoneme_pauses = [' ', '\'', ',', '.']
voices = [
0, #Perfect Paul
1, #Vader
2, #Big Bob
3, #Precise Pete
4, #Ricochet Randy
5, #Biff
6, #Skip
7, #Robo Robert
8, #Goliath
9, #Alvin
10 #Gretchen
]
class uart_session():
	def __init__(self, tx_pin, rx_pin, cts_pin, reset_pin=3, baudrate=115200):
		self.tx_pin = Pin(tx_pin)
		self.rx_pin = Pin(rx_pin)
		self.cts_pin = Pin(cts_pin, mode=Pin.IN)
		self.reset_pin = Pin(reset_pin, mode=Pin.OUT, pull=Pin.PULL_UP)
		#self.reset_pin.off()
		self.control_char = chr(0x01)
		self.term_char = '\r'
		self.session = UART(
			0,
			baudrate=baudrate,
			tx=tx_pin,
			rx=rx_pin,
			cts=cts_pin,
			flow=UART.CTS
			)
		self.text_mode = True
		self.phoneme_mode = False
		# Reset the chip before continuing
		self.reset()
	def write_command(self, command):
		"Writes the specified command, encoding to bytes and includes term_char"
		self.session.write((command + self.term_char).encode())
	def reset(self):
		"Resets the RC VStamp"
		self.reset_pin.off()
		time.sleep(0.2)
		self.reset_pin.on()
	def phoneme_mode(self):
		'Switches the RC chip to Phoneme mode'
		self.write_command(control_char + 'd')
		self.phoneme_mode = True
	def text_mode(self):
		self.write_command(control_char + 't')
		self.text_mode = True
print('defs done')
RC = uart_session(0, 1, 2)
print('uart established')
time.sleep(1)
RC.write_command('READY')