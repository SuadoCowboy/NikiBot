import discord
import subprocess
import os
from dotenv import load_dotenv
import commands

load_dotenv()

CHANNEL_ID = 1293321381181788261 # allowed channel to use commands
GUILD_ID = 1293321380863021159
CLIENT_ID = 1346652392350945310

OWNERS_IDS = os.getenv('DISCORD_BOT_OWNERS').split(',')
for i in range(len(OWNERS_IDS)):
	OWNERS_IDS[i] = int(OWNERS_IDS[i])

def isUserIdInOwners(interaction: discord.Interaction):
	return interaction.user.id in OWNERS_IDS

nikicmdProcess = subprocess.Popen(
    ["./NikiCMD"],
    stdin=subprocess.PIPE, 
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE,
    text=True,
)

def handleNikiCMDProcess(script: str, isOwnerId: int):
	if not nikicmdProcess.stdin.writable():
		return ('NikiCMD is not receiving inputs at the moment. Please try again later', [])

	nikicmdProcess.stdin.write(f'{script}{isOwnerId}\n')
	nikicmdProcess.stdin.flush()

	message = ''
	discordCommands = []
	lineBeforeWasPrintLevel = isForDiscord = False
	while True:
		line = nikicmdProcess.stdout.readline()

		if line == 'DISCORD\n' and not lineBeforeWasPrintLevel:
			isForDiscord = True
			continue

		if line == 'END\n' and isForDiscord:
			break

		if isForDiscord:
			discordCommands.append(line[:-1])
		else:
			message += line

		if line in ('ECHO:\n', 'WARNING:\n', 'ERROR:\n', 'DEFAULT:\n'):
			lineBeforeWasPrintLevel = True
		else:
			lineBeforeWasPrintLevel = False

	return (message, discordCommands)

async def parseDiscordCommands(interaction: discord.Interaction, discordCommands: list[str], isOwner: bool):
	for command in discordCommands:
		if len(command.replace(' ', '')) == 0:
			continue

		args = command.split(' ')

		command = args[0]
		if len(args) == 1:
			args.clear()
		else:
			args = args[1:]

		if command in commands.commands:
			await commands.commands[command](interaction, command, args, isOwner)
		elif interaction.app_permissions.send_messages:
			await interaction.channel.send(f"Received unknown command {command} with those arguments: {args}")

async def runNikiScript(interaction: discord.Interaction, script: str):
	embed = discord.Embed(color=discord.Colour(0xffc6ff))

	if len(script) > 1017: # counting ````` => -6
		await interaction.response.edit_message(content="Script must be 1017 or fewer in length")
		return

	embed.add_field(name="In", value=f'```{script}```', inline=False)

	isOwner = isUserIdInOwners(interaction)
	message = ''
	discordCommands = []
	try:
		message, discordCommands = handleNikiCMDProcess(script, 1 if isOwner else 0)
	except subprocess.TimeoutExpired:
		await interaction.edit_original_response("Execution timed out!")
		return
	except Exception as e:
		await interaction.edit_original_response(f"Error: {e}")
		return

	if len(message) > 1017: # counting ````` => -6
		message = message[:1017]

	embed.add_field(name="Out", value=f'```{message}```', inline=False)

	await interaction.edit_original_response(embed=embed)
	if len(discordCommands) != 0:
			await parseDiscordCommands(interaction, discordCommands, isOwner)

class NikiBot(discord.Client):
	async def on_ready(self):
		await tree.sync(guild=discord.Object(GUILD_ID))
		await tree.sync()
		print(f'Logged on as {self.user}!')

intents = discord.Intents.default()
intents.message_content = True
intents.dm_messages = True

client = NikiBot(intents=intents)
tree = discord.app_commands.CommandTree(client)

@tree.command(
    name="nikiscript",
    description="Interprets text as NikiScript",
	guild=discord.Object(GUILD_ID)
)
async def script(interaction: discord.Interaction, script: str):
	if not isUserIdInOwners(interaction) and interaction.channel_id != CHANNEL_ID:
		await interaction.response.send_message(f"Can not run nikiscript here. Try on <#{CHANNEL_ID}> instead.", ephemeral=True)
		return

	await interaction.response.defer()
	await runNikiScript(interaction, script);

@tree.command(
	name='nikiscript2',
	description="Interprets text as NikiScript"
)
@discord.app_commands.check(isUserIdInOwners)
@discord.app_commands.allowed_contexts(guilds=True, dms=True, private_channels=True)
async def script(interaction: discord.Interaction, script: str, hidden: bool=True):
	await interaction.response.defer(ephemeral=hidden)
	await runNikiScript(interaction, script)

client.run(os.getenv('DISCORD_BOT_TOKEN'))