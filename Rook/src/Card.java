import java.util.Map;

public class Card {
    public static final Map<String, Integer> pointMap = Map.of("1", 15, "14",
            10, "10", 10, "5", 5, "rook", 20);
    public static final Map<String, Integer> valueExceptions = Map.of("1", 15,
            "rook", 1);
    public static final String[] suits = { "red", "yellow", "black", "green" };
    public static final String[] ranks = { "2", "3", "4", "5", "6", "7", "8",
            "9", "10", "11", "12", "13", "14", "1" };
    public static Card[] ALL_CARDS;
    static {
        ALL_CARDS = new Card[suits.length * ranks.length + 1];
        ALL_CARDS[0] = new Card("rook", "rook");
        int index = 1;
        for (String suit : suits) {
            for (String rank : ranks) {
                ALL_CARDS[index] = new Card(suit, rank);
                index++;
            }
        }
    }
    public String suit;
    public String rank;
    public int points;
    public int value;

    public String toString() {
        return rank.equals("rook") ? "rook" : this.suit + " " + this.rank;
    }

    // 1 if I win, 0 if equal, -1 if he wins
    public int compare(Card card, String trump) {
        boolean iAmTrump = suit.equals(trump) || suit.equals("rook");
        boolean heIsTrump = card.suit.equals(trump) || card.suit.equals("rook");
        if (iAmTrump && !heIsTrump) {
            return 1;
        }
        if (heIsTrump && !iAmTrump) {
            return -1;
        }
        if (value == card.value) {
            return 0;
        }
        return value > card.value ? 1 : -1;
    }

    public Card(String suit, String rank) {
        this.suit = suit;
        this.rank = rank;
        this.points = pointMap.containsKey(rank) ? pointMap.get(rank) : 0;
        this.value = valueExceptions.containsKey(rank)
                ? valueExceptions.get(rank)
                : Integer.parseInt(rank, 10);
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof Card)) {
            return false;
        }
        Card c = (Card) o;
        return c.toString().equals(toString());
    }
}
