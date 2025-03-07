import discord
import subprocess

nikicmdProcess = None

def startNikiCMD():
    global nikicmdProcess

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

async def parseDiscordCommands(interaction: discord.Interaction, discordCommands: list[str], isUserIdInOwners: bool):
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
			await commands.commands[command](interaction, command, args, isUserIdInOwners)
		elif interaction.app_permissions.send_messages:
			await interaction.channel.send(f"Received unknown command {command} with those arguments: {args}")

async def runNikiScript(interaction: discord.Interaction, script: str):
	embed = discord.Embed(color=discord.Colour(0xffc6ff))

	if len(script) > 1017: # counting ````` => -6
		await interaction.response.edit_message(content="Script must be 1017 or fewer in length")
		return

	embed.add_field(name="In", value=f'```{script}```', inline=False)

	isUserIdInOwners = isOwner(interaction)
	message = ''
	discordCommands = []
	try:
		message, discordCommands = handleNikiCMDProcess(script, 1 if isUserIdInOwners else 0)
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
			await parseDiscordCommands(interaction, discordCommands, isUserIdInOwners)