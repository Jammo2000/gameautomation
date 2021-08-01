import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Scanner;

public class Agent {
    public Player[] players = new Player[4];
    Scanner input = new Scanner(System.in);
    Player me;
    ArrayList<Card> myCards;
    ArrayList<Card> unseenCards = new ArrayList<>(
            (Arrays.asList(Card.ALL_CARDS)));

    private String pickTrump(ArrayList<Card> hand) {
        int[] suitValues = new int[Card.suits.length];
        for (int i = 0; i < Card.suits.length; i++) {
            for (Card card : hand) {
                if (card.suit == Card.suits[i]) {
                    suitValues[i] += card.value;
                }
            }
        }
        int maxind = 0;
        for (int i = 0; i < suitValues.length; i++) {
            if (suitValues[i] > suitValues[maxind]) {
                maxind = i;
            }
        }
        return Card.suits[maxind];
    }

    // get the number of unseencards that this card beats
    private int getBeatenCards(Card card, String trump) {
        int beatenCount = 0;
        for (Card comparing : unseenCards) {
            if (card.compare(comparing, trump) == 1) {
                beatenCount++;
            }
        }
        return beatenCount;
    }

    private int getMaxBet() {
        int count = 0;
        String trump = pickTrump(myCards);

        for (Card card : myCards) {
            count += getBeatenCards(card, trump) + card.points;
        }
        return count * 200 / 584;
    }

    public ArrayList<Card> getCards(int count) {
        ArrayList<Card> cards = new ArrayList<>(count);
        for (int i = 0; i < count; i++) {
            System.out.println("Enter card #" + (i + 1)
                    + " in format 'color rank', or 'rook'");
            String currentCard = input.nextLine();
            try {
                if (currentCard.equals("rook")) {
                    cards.add(new Card("rook", "rook"));
                } else {
                    int spaceIndex = currentCard.indexOf(' ');
                    String suit = currentCard.substring(0, spaceIndex);
                    String rank = currentCard.substring(spaceIndex + 1);
                    cards.add(new Card(suit, rank));
                }
            } catch (Exception e) {
                System.out.println("Invalid input");
                i--;
            }
        }
        return cards;
    }

    private int howManyBidders() {
        int count = 0;
        for (Player player : players) {
            if (player.bid > -1) {
                count++;
            }
        }
        return count;
    }

    public void play() {
        // populate players array
        Player me = new Player("me");
        players[0] = me;
        for (int i = 1; i < 4; i++) {
            System.out.println(
                    "Enter name of player " + i + ", starting from my left");
            players[i] = new Player(input.nextLine().toLowerCase());
        }
        // get hand
        System.out.println("Enter r to randomize hand, or i to input");
        String randomize = input.nextLine();
        if (randomize.equals("r")) {
            myCards = new ArrayList<Card>();
            for (int i = 0; i < 13; i++) {
                int index = (int) Math
                        .floor(Math.random() * Card.ALL_CARDS.length);
                myCards.add(Card.ALL_CARDS[index]);
            }
            myCards.sort(new Comparator<Card>() {
                @Override
                public int compare(Card o1, Card o2) {
                    if (o1.suit.compareTo(o2.suit) != 0) {
                        return o1.suit.compareTo(o2.suit);
                    }
                    return o1.rank.compareTo(o2.rank);
                }
            });
            for (Card card : myCards) {
                System.out.println(card);
            }
        } else {
            myCards = getCards(13);
        }
        System.out.println(unseenCards.removeAll(myCards));
        System.out.println(getMaxBet());
        System.out.println(pickTrump(myCards));
        System.out.println();
        // bidding
        System.out.println("Who bids first? (fyi, my name is 'me')");
        String firstBidderName = input.nextLine().toLowerCase();
        int bidderIndex = 0;
        for (int i = 0; i < players.length; i++) {
            if (players[i].name.equals(firstBidderName)) {
                bidderIndex = i;
                break;
            }
        }
        int maxBid = 0;
        while (howManyBidders() > 1) {
            if (players[bidderIndex].bid != -1) {
                int newBid;
                if (bidderIndex == 0) {// i'm bidding
                    if (maxBid > getMaxBet()) {
                        players[0].bid = -1;
                        System.out.println("I pass");
                        newBid = -1;
                    } else {
                        newBid = maxBid + 5;
                        System.out.println("My bid is " + newBid);
                    }
                } else {
                    newBid = players[bidderIndex].getBid();
                }
                if (newBid > maxBid) {
                    maxBid = newBid;
                }
            }
            bidderIndex = (bidderIndex + 1) % players.length;
        }
        Player winner = null;
        for (Player player : players) {
            if (player.bid != -1) {
                winner = player;
                break;
            }
        }
    }
}
