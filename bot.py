import discord
import subprocess
import os
from dotenv import load_dotenv

load_dotenv()

nikicmdProcess = subprocess.Popen(
    ["./NikiCMD"],
    stdin=subprocess.PIPE, 
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE,
    text=True,
)

async def runNikiScript(script: str):
	while True:
		output = nikicmdProcess.stdout.readline()
		if not output:
			return "NikiCMD is not receiving input. Try again later"

		if output.endswith('input\n'):
			break

	nikicmdProcess.stdin.write(script+'\n')
	nikicmdProcess.stdin.flush()

	message = ''
	while True:
		output = nikicmdProcess.stdout.readline()
		if output == 'end\n':
			break

		if not output or output == '\n':
			continue

		message += output

	return message

class NikiBot(discord.Client):
	async def on_ready(self):
		print(f'Logged on as {self.user}!')

	async def on_message(self, message):
		if not message.content.startswith('.'):
			return

		args = message.content.split(' ')

		command = args[0]
		if len(command) == 1:
			return
		command = command[1:]

		if len(args) > 0:
			args = args[1:]

		if command == 'niki':
			await message.channel.send('https://github.com/SuadoCowboy/NikiScript')

		elif command == 'script' and len(args) != 0:
			try:
				inputMessage = message.content[8:]
				response = '**IN:**\n```' + inputMessage + '```\n**OUT:**```cpp\n' + await runNikiScript(inputMessage) + '```'

				await message.channel.send(response)
			except subprocess.TimeoutExpired:
				await message.channel.send("Execution timed out!")
			except Exception as e:
				await message.channel.send(f"Error: {e}")

intents = discord.Intents.default()
intents.message_content = True

client = NikiBot(intents=intents)
client.run(os.getenv('DISCORD_BOT_TOKEN'))
