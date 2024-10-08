PGDMP     .    #                {            Wedding Shop    10.18    10.18 9    F           0    0    ENCODING    ENCODING        SET client_encoding = 'UTF8';
                       false            G           0    0 
   STDSTRINGS 
   STDSTRINGS     (   SET standard_conforming_strings = 'on';
                       false            H           0    0 
   SEARCHPATH 
   SEARCHPATH     8   SELECT pg_catalog.set_config('search_path', '', false);
                       false            I           1262    33791    Wedding Shop    DATABASE     �   CREATE DATABASE "Wedding Shop" WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'English_United States.1252' LC_CTYPE = 'English_United States.1252';
    DROP DATABASE "Wedding Shop";
             postgres    false                        2615    2200    public    SCHEMA        CREATE SCHEMA public;
    DROP SCHEMA public;
             postgres    false            J           0    0    SCHEMA public    COMMENT     6   COMMENT ON SCHEMA public IS 'standard public schema';
                  postgres    false    3                        3079    12924    plpgsql 	   EXTENSION     ?   CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;
    DROP EXTENSION plpgsql;
                  false            K           0    0    EXTENSION plpgsql    COMMENT     @   COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';
                       false    1            �            1259    33843    cart_elements    TABLE     �   CREATE TABLE public.cart_elements (
    id integer NOT NULL,
    user_id integer NOT NULL,
    product_id integer NOT NULL,
    date date DEFAULT CURRENT_DATE NOT NULL
);
 !   DROP TABLE public.cart_elements;
       public         postgres    false    3            �            1259    33841    cart_elements_id_seq    SEQUENCE     �   ALTER TABLE public.cart_elements ALTER COLUMN id ADD GENERATED ALWAYS AS IDENTITY (
    SEQUENCE NAME public.cart_elements_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);
            public       postgres    false    3    205            �            1259    33860    collection_elements    TABLE     �   CREATE TABLE public.collection_elements (
    id integer NOT NULL,
    user_id integer NOT NULL,
    product_id integer NOT NULL,
    date date DEFAULT CURRENT_DATE NOT NULL
);
 '   DROP TABLE public.collection_elements;
       public         postgres    false    3            �            1259    33858    collection_elements_id_seq    SEQUENCE     �   ALTER TABLE public.collection_elements ALTER COLUMN id ADD GENERATED ALWAYS AS IDENTITY (
    SEQUENCE NAME public.collection_elements_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);
            public       postgres    false    207    3            �            1259    41999    color    TABLE     �   CREATE TABLE public.color (
    id integer NOT NULL,
    name character varying(128) NOT NULL,
    date date DEFAULT CURRENT_DATE NOT NULL
);
    DROP TABLE public.color;
       public         postgres    false    3            �            1259    41997    color_id_seq    SEQUENCE     �   ALTER TABLE public.color ALTER COLUMN id ADD GENERATED ALWAYS AS IDENTITY (
    SEQUENCE NAME public.color_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);
            public       postgres    false    211    3            �            1259    41985    material    TABLE     �   CREATE TABLE public.material (
    id integer NOT NULL,
    name character varying(128) NOT NULL,
    date date DEFAULT CURRENT_DATE NOT NULL
);
    DROP TABLE public.material;
       public         postgres    false    3            �            1259    41983    material_id_seq    SEQUENCE     �   ALTER TABLE public.material ALTER COLUMN id ADD GENERATED ALWAYS AS IDENTITY (
    SEQUENCE NAME public.material_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);
            public       postgres    false    209    3            �            1259    33799    product    TABLE     ?  CREATE TABLE public.product (
    id integer NOT NULL,
    seller_id integer NOT NULL,
    material_id integer NOT NULL,
    color_id integer NOT NULL,
    name character varying(128) NOT NULL,
    price integer NOT NULL,
    description character varying(1024) NOT NULL,
    date date DEFAULT CURRENT_DATE NOT NULL
);
    DROP TABLE public.product;
       public         postgres    false    3            �            1259    33797    product_id_seq    SEQUENCE     �   ALTER TABLE public.product ALTER COLUMN id ADD GENERATED ALWAYS AS IDENTITY (
    SEQUENCE NAME public.product_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);
            public       postgres    false    198    3            �            1259    33823    review    TABLE     �   CREATE TABLE public.review (
    id integer NOT NULL,
    product_id integer NOT NULL,
    user_id integer NOT NULL,
    rating integer NOT NULL,
    content character varying(1024),
    date date DEFAULT CURRENT_DATE NOT NULL
);
    DROP TABLE public.review;
       public         postgres    false    3            �            1259    33821    review_id_seq    SEQUENCE     �   ALTER TABLE public.review ALTER COLUMN id ADD GENERATED ALWAYS AS IDENTITY (
    SEQUENCE NAME public.review_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);
            public       postgres    false    203    3            �            1259    33792    seller    TABLE     �   CREATE TABLE public.seller (
    id integer NOT NULL,
    email character varying(128) NOT NULL,
    password character varying(16) NOT NULL,
    name character varying(16) NOT NULL,
    date date DEFAULT CURRENT_DATE NOT NULL
);
    DROP TABLE public.seller;
       public         postgres    false    3            �            1259    33812    seller_id_seq    SEQUENCE     �   ALTER TABLE public.seller ALTER COLUMN id ADD GENERATED ALWAYS AS IDENTITY (
    SEQUENCE NAME public.seller_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);
            public       postgres    false    3    196            �            1259    33816    user    TABLE     �   CREATE TABLE public."user" (
    id integer NOT NULL,
    email character varying(128) NOT NULL,
    password character varying(16) NOT NULL,
    name character varying(128) NOT NULL,
    date date DEFAULT CURRENT_DATE NOT NULL
);
    DROP TABLE public."user";
       public         postgres    false    3            �            1259    33814    user_id_seq    SEQUENCE     �   ALTER TABLE public."user" ALTER COLUMN id ADD GENERATED ALWAYS AS IDENTITY (
    SEQUENCE NAME public.user_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1
);
            public       postgres    false    3    201            =          0    33843    cart_elements 
   TABLE DATA               F   COPY public.cart_elements (id, user_id, product_id, date) FROM stdin;
    public       postgres    false    205   �@       ?          0    33860    collection_elements 
   TABLE DATA               L   COPY public.collection_elements (id, user_id, product_id, date) FROM stdin;
    public       postgres    false    207   �@       C          0    41999    color 
   TABLE DATA               /   COPY public.color (id, name, date) FROM stdin;
    public       postgres    false    211   1A       A          0    41985    material 
   TABLE DATA               2   COPY public.material (id, name, date) FROM stdin;
    public       postgres    false    209   �A       6          0    33799    product 
   TABLE DATA               g   COPY public.product (id, seller_id, material_id, color_id, name, price, description, date) FROM stdin;
    public       postgres    false    198   aB       ;          0    33823    review 
   TABLE DATA               P   COPY public.review (id, product_id, user_id, rating, content, date) FROM stdin;
    public       postgres    false    203   dC       4          0    33792    seller 
   TABLE DATA               A   COPY public.seller (id, email, password, name, date) FROM stdin;
    public       postgres    false    196   �C       9          0    33816    user 
   TABLE DATA               A   COPY public."user" (id, email, password, name, date) FROM stdin;
    public       postgres    false    201   BD       L           0    0    cart_elements_id_seq    SEQUENCE SET     C   SELECT pg_catalog.setval('public.cart_elements_id_seq', 22, true);
            public       postgres    false    204            M           0    0    collection_elements_id_seq    SEQUENCE SET     I   SELECT pg_catalog.setval('public.collection_elements_id_seq', 13, true);
            public       postgres    false    206            N           0    0    color_id_seq    SEQUENCE SET     :   SELECT pg_catalog.setval('public.color_id_seq', 8, true);
            public       postgres    false    210            O           0    0    material_id_seq    SEQUENCE SET     =   SELECT pg_catalog.setval('public.material_id_seq', 7, true);
            public       postgres    false    208            P           0    0    product_id_seq    SEQUENCE SET     <   SELECT pg_catalog.setval('public.product_id_seq', 8, true);
            public       postgres    false    197            Q           0    0    review_id_seq    SEQUENCE SET     ;   SELECT pg_catalog.setval('public.review_id_seq', 4, true);
            public       postgres    false    202            R           0    0    seller_id_seq    SEQUENCE SET     ;   SELECT pg_catalog.setval('public.seller_id_seq', 5, true);
            public       postgres    false    199            S           0    0    user_id_seq    SEQUENCE SET     9   SELECT pg_catalog.setval('public.user_id_seq', 6, true);
            public       postgres    false    200            �
           2606    33847     cart_elements cart_elements_pkey 
   CONSTRAINT     ^   ALTER TABLE ONLY public.cart_elements
    ADD CONSTRAINT cart_elements_pkey PRIMARY KEY (id);
 J   ALTER TABLE ONLY public.cart_elements DROP CONSTRAINT cart_elements_pkey;
       public         postgres    false    205            �
           2606    33864 ,   collection_elements collection_elements_pkey 
   CONSTRAINT     j   ALTER TABLE ONLY public.collection_elements
    ADD CONSTRAINT collection_elements_pkey PRIMARY KEY (id);
 V   ALTER TABLE ONLY public.collection_elements DROP CONSTRAINT collection_elements_pkey;
       public         postgres    false    207            �
           2606    42004    color color_pkey 
   CONSTRAINT     N   ALTER TABLE ONLY public.color
    ADD CONSTRAINT color_pkey PRIMARY KEY (id);
 :   ALTER TABLE ONLY public.color DROP CONSTRAINT color_pkey;
       public         postgres    false    211            �
           2606    41989    material material_pkey 
   CONSTRAINT     T   ALTER TABLE ONLY public.material
    ADD CONSTRAINT material_pkey PRIMARY KEY (id);
 @   ALTER TABLE ONLY public.material DROP CONSTRAINT material_pkey;
       public         postgres    false    209            �
           2606    33806    product product_pkey 
   CONSTRAINT     R   ALTER TABLE ONLY public.product
    ADD CONSTRAINT product_pkey PRIMARY KEY (id);
 >   ALTER TABLE ONLY public.product DROP CONSTRAINT product_pkey;
       public         postgres    false    198            �
           2606    33830    review review_pkey 
   CONSTRAINT     P   ALTER TABLE ONLY public.review
    ADD CONSTRAINT review_pkey PRIMARY KEY (id);
 <   ALTER TABLE ONLY public.review DROP CONSTRAINT review_pkey;
       public         postgres    false    203            �
           2606    33796    seller seller_pkey 
   CONSTRAINT     P   ALTER TABLE ONLY public.seller
    ADD CONSTRAINT seller_pkey PRIMARY KEY (id);
 <   ALTER TABLE ONLY public.seller DROP CONSTRAINT seller_pkey;
       public         postgres    false    196            �
           2606    33820    user user_pkey 
   CONSTRAINT     N   ALTER TABLE ONLY public."user"
    ADD CONSTRAINT user_pkey PRIMARY KEY (id);
 :   ALTER TABLE ONLY public."user" DROP CONSTRAINT user_pkey;
       public         postgres    false    201            �
           2606    42028    product color_id    FK CONSTRAINT     �   ALTER TABLE ONLY public.product
    ADD CONSTRAINT color_id FOREIGN KEY (color_id) REFERENCES public.color(id) ON UPDATE CASCADE ON DELETE CASCADE NOT VALID;
 :   ALTER TABLE ONLY public.product DROP CONSTRAINT color_id;
       public       postgres    false    211    198    2737            �
           2606    42023    product material_id    FK CONSTRAINT     �   ALTER TABLE ONLY public.product
    ADD CONSTRAINT material_id FOREIGN KEY (material_id) REFERENCES public.material(id) ON UPDATE CASCADE ON DELETE CASCADE NOT VALID;
 =   ALTER TABLE ONLY public.product DROP CONSTRAINT material_id;
       public       postgres    false    198    2735    209            �
           2606    33831    review product_id    FK CONSTRAINT     �   ALTER TABLE ONLY public.review
    ADD CONSTRAINT product_id FOREIGN KEY (product_id) REFERENCES public.product(id) ON UPDATE CASCADE ON DELETE CASCADE;
 ;   ALTER TABLE ONLY public.review DROP CONSTRAINT product_id;
       public       postgres    false    2725    203    198            �
           2606    33853    cart_elements product_id    FK CONSTRAINT     �   ALTER TABLE ONLY public.cart_elements
    ADD CONSTRAINT product_id FOREIGN KEY (product_id) REFERENCES public.product(id) ON UPDATE CASCADE ON DELETE CASCADE;
 B   ALTER TABLE ONLY public.cart_elements DROP CONSTRAINT product_id;
       public       postgres    false    205    2725    198            �
           2606    33870    collection_elements product_id    FK CONSTRAINT     �   ALTER TABLE ONLY public.collection_elements
    ADD CONSTRAINT product_id FOREIGN KEY (product_id) REFERENCES public.product(id) ON UPDATE CASCADE ON DELETE CASCADE;
 H   ALTER TABLE ONLY public.collection_elements DROP CONSTRAINT product_id;
       public       postgres    false    2725    198    207            �
           2606    33807    product seller_id    FK CONSTRAINT     �   ALTER TABLE ONLY public.product
    ADD CONSTRAINT seller_id FOREIGN KEY (seller_id) REFERENCES public.seller(id) ON UPDATE CASCADE ON DELETE CASCADE;
 ;   ALTER TABLE ONLY public.product DROP CONSTRAINT seller_id;
       public       postgres    false    2723    198    196            �
           2606    33836    review user_id    FK CONSTRAINT     �   ALTER TABLE ONLY public.review
    ADD CONSTRAINT user_id FOREIGN KEY (user_id) REFERENCES public."user"(id) ON UPDATE CASCADE ON DELETE CASCADE;
 8   ALTER TABLE ONLY public.review DROP CONSTRAINT user_id;
       public       postgres    false    201    203    2727            �
           2606    33848    cart_elements user_id    FK CONSTRAINT     �   ALTER TABLE ONLY public.cart_elements
    ADD CONSTRAINT user_id FOREIGN KEY (user_id) REFERENCES public."user"(id) ON UPDATE CASCADE ON DELETE CASCADE;
 ?   ALTER TABLE ONLY public.cart_elements DROP CONSTRAINT user_id;
       public       postgres    false    2727    201    205            �
           2606    33865    collection_elements user_id    FK CONSTRAINT     �   ALTER TABLE ONLY public.collection_elements
    ADD CONSTRAINT user_id FOREIGN KEY (user_id) REFERENCES public."user"(id) ON UPDATE CASCADE ON DELETE CASCADE;
 E   ALTER TABLE ONLY public.collection_elements DROP CONSTRAINT user_id;
       public       postgres    false    2727    207    201            =   '   x�32�4�4�4202�50�54�22�#DL�b���� �0[      ?   N   x�]˱�0C�vq&�.��*�R�@NJ*jC怰v�T�^��/�70^�� �8 �(E{Y��{1�*G#�      C   �   x�e�1
�@E��Sx�H�$��x!����+����6�3���i�3�����3=-!����m��efkc�/�6�SVL���R��ڶg��J�k��O}x�ң��S�ꕎ���X�R��0���yۥ1�/Vf�      A   �   x�e��
�@D����K��/~Lr
"�6���e�$�3�B�6[�7��Zб�o�xE��"���U�wz|ѳIy!xa�	S�K�Ӳ�o��6���b~�g+x����ݞ��%;[�hv^S���JUTtZ{      6   �   x���Mn�0���S�A�I�Y�$%��W�lA�q@�TJ!Wx�Q'i+� %��5�<o����v8b�%���R��{�p+n�g*ϒ	��[H����>Xy�{MF�)�3��S�5�<��l�ƯÜty�i%,����hG����n��Y��L�4�)�F�h<J��k�a��bз����3���䟩/�o�ny�eN�o��))n��n4��sy���Nn��=!��EM      ;   c   x�3�4�4�44�p�w�q�4202�50�54�2�rZp��(�䗥*�dd+��+$EJ��2g�4I�h�!�%'�X� /      4   [   x�3���/�H-�q�K�+*�442�tILBN##c]]#.S�$�����
G_G?G�
S]K.�D.�A��z��Es,�b���� ܈X      9   �   x�uN�
�@]��w�����;�W�� hs�AGq���� j���yr$0sf��{�T��A�JGq)�)pQ�*����q�Ot�k��;�Y�IV/��`��?ֻ�FRcl�k����7~�]�OLy1��:�x���욉�_oS�	D|*�;+     