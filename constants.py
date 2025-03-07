import subprocess
import discord
from dotenv import load_dotenv

load_dotenv()

client: discord.Client = None

CHANNEL_ID = 1293321381181788261 # allowed channel to use commands
GUILD_ID = 1293321380863021159
CLIENT_ID = 1346652392350945310

OWNERS_IDS = os.getenv('DISCORD_BOT_OWNERS').split(',')
for i in range(len(OWNERS_IDS)):
	OWNERS_IDS[i] = int(OWNERS_IDS[i])

def isOwner(interaction: discord.Interaction):
	return interaction.user.id in OWNERS_IDS