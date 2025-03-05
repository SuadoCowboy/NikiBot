import discord
import subprocess
import os
from dotenv import load_dotenv

load_dotenv()

CHANNEL_ID = 1293321381181788261 # allowed channel to use commands
GUILD_ID = 1293321380863021159
OWNER_ID = 309806482084462592
CLIENT_ID = 1346652392350945310

def isOwner(interaction: discord.Interaction):
	return interaction.user.id == OWNER_ID

nikicmdProcess = subprocess.Popen(
    ["./NikiCMD"],
    stdin=subprocess.PIPE, 
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE,
    text=True,
)

def handleNikiCMDProcess(script: str):
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

def runNikiScript(script: str):
	try:
		return handleNikiCMDProcess(script)
	except subprocess.TimeoutExpired:
		return "Execution timed out!"
	except Exception as e:
		return f"Error: {e}"

class NikiBot(discord.Client):
	async def on_ready(self):
		await tree.sync()
		print(f'Logged on as {self.user}!')

intents = discord.Intents.default()
intents.message_content = True
intents.dm_messages = True

client = NikiBot(intents=intents)
tree = discord.app_commands.CommandTree(client)

@tree.command(
    name="nikiscript",
    description="Interprets text as NikiScript"
)
async def script(interaction: discord.Interaction, text: str):
	if interaction.user.id != OWNER_ID:
		if interaction.guild_id != GUILD_ID:
			await interaction.response.send_message("Can't run outside JIKARU lol. Do the L.", ephemeral=True)
			return

		elif interaction.channel_id != CHANNEL_ID:
			await interaction.response.send_message(f"Can not run nikiscript here. Try on <#{CHANNEL_ID}> instead.", ephemeral=True)
			return

	await interaction.response.defer(ephemeral=interaction.channel_id != CHANNEL_ID, thinking=True)

	embed = discord.Embed(color=discord.Colour(0xffc6ff))
	embed.add_field(name="In", value=f'```{text}```', inline=False)
	embed.add_field(name="Out", value=f'```{runNikiScript(text)}```', inline=False)

	await interaction.edit_original_response(embed=embed)

client.run(os.getenv('DISCORD_BOT_TOKEN'))