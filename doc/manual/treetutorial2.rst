<chapter id="treetutorial2">
<title id="treetutorial2.title">Modifying Tree Nodes</title>

<section>

<title></title>

<para>
	Now that we have seen in <xref linkend="treetutorial1"> how to inspect the
	tree, in this tutorial we will look at modifying the tree. The task we set
	ourselves is: replace all calls to ``mysql_connect`` by calls to
	``dbx_connect`` (<ulink
	url="http://pecl.php.net/package/dbx">dbx</ulink> is a PECL extension to PHP
	that allows scripts interface with a database independent of the type of the
	database; this conversion could be part of a larger refactoring process that
	makes a script written for MySQL work with other databases.) 
</para>

<para>
	The tutorial we develop in this tutorial is available as
	<filename>MySQL2DBX.la</filename> in the &phc distribution. To see its
	effect, run &phc as follows: 
</para>
	
<programlisting>
phc --run plugins/tutorials/MySQL2DBX.la --pretty-print test.php
</programlisting>

</section>
<section>

<title>First Attempt</title>

<para>
	We are interested in all function calls to ``mysql_connect``.  Let
	us have a look at the precise definition of a function call according to the
	<xref linkend="grammar" endterm="grammar.title">: 
</para>

<programlisting>
Method_invocation ::= Target? Method_name Actual_parameter* ;
Method_name ::= METHOD_NAME | Reflection ;
Actual_parameter ::= is_ref:"&amp;"? Expr ;
Reflection ::= Expr ;
</programlisting>
			
<para>
	(The ``target`` of a method invocation is the class or object the
	function gets invoked on, if any. It need not worry us here.) For now, we
	are only interested in the ``Method_name``. The grammar tells us
	that a ``Method_name`` is either a ``METHOD_NAME`` or a
	node of type ``Reflection``. If a symbol is written in CAPITALS in
	the grammar, that means it refers to a &ldquo;token&rdquo;, a literal value.
	In this case, to an actual method name (such as ``mysql_connect``).
	In PHP, it is also possible to call a method whose name is stored in
	variable; in this case, the function name will be a ``Reflection``
	node (which contains an ``Expr``). In this tutorial, we are
	interested in &ldquo;normal&rdquo; method invocations only.  
</para>

<para>
	All tokens have an attribute called ``value``
	which corresponds to the value of the token. For most tokens, the type of
	``value`` is a ``String*`` (consider a ``String``
	to be an STL ``string``. However, for some tokens, for example
	``INT``, ``value`` has a different type (e.g.,
	``int``). If the token has a non-standard type, it will have method 
	called ``get_source_rep``, which returns a ``String*``
	representing the token in the source. For example, the real number
	``5E-1`` would have ``value`` equal to the
	(``double``) 0.5, but ``get_source_rep`` would return (the
	``String*``) &ldquo;5E-1&rdquo;. 
</para> 

<para>
	Thus, we arrive at the following first attempt. 
</para>

<programlisting>
<reserved>#include</reserved> "AST_visitor.h"
<reserved>#include</reserved> &lt;pass_manager/Plugin_pass.h&gt;

<reserved>using</reserved> <reserved>namespace</reserved> AST;

<reserved>class</reserved> MySQL2DBX : <reserved>public</reserved> Visitor
{
<reserved>public</reserved>:
   <reserved>void</reserved> post_method_invocation(Method_invocation* in)
   {
      <reserved>if</reserved>(in->method_name->match(<reserved>new</reserved> METHOD_NAME(<reserved>new</reserved> String("mysql_connect"))))
      {
         <emphasis>// Modify name</emphasis>
         in->method_name = <reserved>new</reserved> METHOD_NAME(<reserved>new</reserved> String("dbx_connect"));
      }
   }
};

<reserved>extern</reserved> "C" <reserved>void</reserved> load (Pass_manager* pm, Plugin_pass* pass)
{
   pm->add_after_named_pass (pass, new String ("ast"));
}

<reserved>extern</reserved> "C" <reserved>void</reserved> run_ast (PHP_script* in, Pass_manager* pm, String* option)
{
   MySQL2DBX m2d;
   in->visit(&amp;m2d);
}
</programlisting>

<note><para>
	&phc; uses a garbage collector, so there is never any need to free objects
	(you never have to call ``delete``).  This makes programming much
	easier and less error-prone (smaller chance of bugs). 
</para></note>

<para>
	``match`` compares two (sub)trees for deep equality.  There is also
	another function called ``deep_equals``, which does nearly the same
	thing, but there are two important differences.  ``match`` does not
	take comments, line numbers and other &ldquo;additional&rdquo; information
	into account, whereas ``deep_equals`` does. The second difference
	is that ``match`` supports wildcards; this will be explained in
	<xref linkend="treetutorial3">.
</para>

</section>
<section>

<title>Modifying the Parameters</title>

<para>
	Unfortunately, renaming ``mysql_connect`` to
	``dbx_connect`` is not sufficient, because the parameters to the
	two functions differ. According to the <ulink
	url="http://www.php.net/manual/en/index.php">PHP manual</ulink>, the
	signatures for both functions are 
</para>

<programlisting>
mysql_connect (server, username, password, new_link, int client_flags)
</programlisting>

<para>
	and 
</para>

<programlisting>
dbx_connect (module, host, database, username, password, persistent)
</programlisting>

<para>
	The ``module`` parameter to ``dbx_connect`` should be set
	to ``DBX_MYSQL`` to connect to a MySQL database. Then
	``host`` corresponds to ``server``, and
	``username`` and ``password`` have the same purpose too.
	So, we should insert ``DBX_MYSQL`` at the front of the list, and
	insert ``NULL`` in between ``host`` and
	``username`` (the ``mysql_connect`` command does not
	select a database). The last two parameters to ``mysql_connect`` do
	not have an equivalent in ``dbx_connect``, so if they are
	specified, we cannot perform the conversion. The last parameter to
	``dbx_connect`` (``persistent``) is optional, and we will
	ignore it in this tutorial.  
</para>

<para>
	Now, in &phc, ``DBX_MYSQL`` is a ``Constant``, which has
	two fields, an optional class name (for class constants) and the name of the
	constant, of type ``CONSTANT_NAME``. ``NULL`` is
	represented by ``NIL`` (to avoid getting confused with the C++
	value ``NULL``.
</para>

<para>
	We are now ready to write our conversion function: 
</para>

<programlisting>
<reserved>#include</reserved> "AST_visitor.h"
<reserved>#include</reserved> &lt;pass_manager/Plugin_pass.h&gt;

<reserved>using</reserved> <reserved>namespace</reserved> AST;

<reserved>class</reserved> MySQL2DBX : <reserved>public</reserved> Visitor
{
<reserved>public</reserved>:
   <reserved>void</reserved> post_method_invocation(Method_invocation* in)
   {
      Actual_parameter_list*&gt;::iterator pos;
      CONSTANT_NAME* module_name;
      Constant* module_constant;
      Actual_parameter* param;
 
      <reserved>if</reserved>(in->method_name->match(<reserved>new</reserved> METHOD_NAME(<reserved>new</reserved> String("mysql_connect"))))
      {
         <emphasis>// Check for too many parameters</emphasis>
         <reserved>if</reserved>(in->actual_parameters->size() > 3)
         {
            printf("Error: unable to translate call "
               "to mysql_connect on line %d\n", in->get_line_number());
            <reserved>return</reserved>;
         }
      
         <emphasis>// Modify name</emphasis>
         in->method_name = <reserved>new</reserved> METHOD_NAME(<reserved>new</reserved> String("dbx_connect"));
      
         <emphasis>// Modify parameters</emphasis>
         module_name = <reserved>new</reserved> CONSTANT_NAME(<reserved>new</reserved> String("DBX_MYSQL"));
         module_constant = <reserved>new</reserved> Constant(NULL, module_name);
         
         pos = in->actual_parameters->begin();
         param = <reserved>new</reserved> Actual_parameter(false, module_constant);
         in->actual_parameters->insert(pos, param); pos++;
         <emphasis>/* Skip host */</emphasis> pos++;
         param = <reserved>new</reserved> Actual_parameter(false, <reserved>new</reserved> NIL());
         in->actual_parameters->insert(pos, param); 
      }
   }
};

<reserved>extern</reserved> "C" <reserved>void</reserved> load (Pass_manager* pm, Plugin_pass* pass)
{
   pm->add_after_named_pass (pass, new String ("ast"));
}

<reserved>extern</reserved> "C" <reserved>void</reserved> run_ast (PHP_script* in, Pass_manager* pm, String* option)
{
	MySQL2DBX m2d;
	in->visit(&amp;m2d);
}
</programlisting>

<para>
	If we apply this transformation to 
</para>

<programlisting>
$link = mysql_connect('host', 'user', 'pass');
</programlisting>

<para>
	We get 
</para>

<programlisting>
$link = dbx_connect(DBX_MYSQL, "host", NULL, "user", "pass");
</programlisting>

</section>
<section>

<title>Refactoring</title>

<para>
	A quick note on refactoring. Refactoring is the process of modifying
	existing programs (PHP scripts), usually to work in new projects or in
	different setups (for example, with a different database engine). Manual
	refactoring is laborious and error-prone, so tool-support is a must.
	Although &phc can be used to refactor PHP code as shown in this tutorial, a
	dedicated refactoring tool for PHP would be easier to use (though of course
	less flexible). Such a tool can however be built on top of &phc;.
</para>

</section>
<section>

<title> What's Next? </title>

<para>
	<xref linkend="treetutorial3"> explains how you can modify the
	<emphasis>structure</emphasis> of the tree, as well as the tree nodes.
</para>

</section>
</chapter>
