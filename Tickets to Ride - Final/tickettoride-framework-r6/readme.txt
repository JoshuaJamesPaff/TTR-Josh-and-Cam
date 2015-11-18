============================================================================
                AI-MAS WINTER OLYMPICS 2011 - Ticket to Ride
============================================================================
                                                          www.aiolympics.ro

R6: 15.02.2011
Ticket to Ride final server and client frameworks.
Important: The server and client frameworks have undergone small but important changes that have made them ready
for the final tournament of the challenge. Please re-update your solutions before submitting their final versions.
Check out the forum for notifications of the latest improvements. 
Many thanks to Matei Tene for his contributions.

														  
R5: 03.02.2011

The new Ticket to Ride server and client applications (both C++ and Java) have undergone extensive improvements.

Important Announcement: 
	Due to very constricted development resources the C++ client for Linux will no longer be maintained. 
	We appologize to any team who has already started developing a solution using that API. The alternative for any such team
	is to use the JAVA client.


Bug fixes from previous version:

		Server
		-------------------------------------------------------------------------------------
		- reject mission fixed
		- all claim routes possibilities have been checked and are working properly
		- checking scores fixed
		- longest route check fixed
		- mission checking & scores fixed
		- longest route score fixed
		- in case the card decks run empty, once cards are discarded the decks are 
			replenished and all players are notified with a Sccccc string - this
			occurs right after someone successfully claimed a route or built a station
		- new autorefill option implemented (see above)
		- discarded cards will be properly placed in a discarded deck that gets reshuffled 
		    when one of the other decks run out of cards
		- shuffle is working as intended now
		- stations will properly give a route only if the route is built by a different player
		- in case the closed deck runs out of cards and there are no discarded cards, and
			someone attempts to claim a tunnel, the server will now return a 7 char string
			containing the information required for the claim attempt, no matter how many
			cards were drawn; eg: the server will send C3D1e00 if only one card was available
			rather than C3D1e - which could have resulted in a block
			
		
		Client
		-------------------------------------------------------------------------------------
		- edge / direct route set owner method fixed
		- claim route methods were checked, fixed and function properly now
		- card counting fixed
		- new nCardsInDeck() method available - returns the number of cards available for draws
		- command queueing working as intended now
		- protocol changed: all client messages that are sent after own turn time has ended
			(be it out of intended or unintended reasons), will be answered with RC (rejected command) 
			rather than being kept for the next round
		- protocol changed: players requesting cards will receive a WD (wrong demand) 
			if they attempt to draw cards and there are no more cards in the stack. 
			If they find themselves in the situation of having no card to draw after
			successfully drawing a card from the closed or open deck, attempting to draw 
			the second card will result in receiving a CiD0 or CiO0 message from the server
			signaling the lack of cards and therefore ending the turn
			
		Important Observation:
			OBS1: all methods return codes - those codes must be checked in order to ensure 
				  that the command was successfull

			OBS2: when attempting to claim a tunnel you might have to recall the same method
				  twice with different parameters if you are required to give the extra cards
				  
		
		
		Map
		-------------------------------------------------------------------------------------
		Due to some issues found in the map xml we have remade the whole xml file from sources given by Days of Wonder (the original game publisher)
		
		
		Graphical User Interface
		-------------------------------------------------------------------------------------
		Fixed issues regarding log parsing.