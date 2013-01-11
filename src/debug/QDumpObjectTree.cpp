#ifndef _QDUMPOBJECTTREE_
#define _QDUMPOBJECTTREE_

#include <QtGui>
namespace {

    void dumpRecursive( QObject * object ,
                        QTreeWidgetItem *parent )
    {
        QTreeWidget *treeWidget = 0;
        QTreeWidgetItem *child;
        if ( parent == 0 ) {
            treeWidget = new QTreeWidget( 0 );
            treeWidget->setWindowTitle("Dumped Object Tree");
            treeWidget->resize(800, 600);
            treeWidget->setRootIsDecorated( TRUE );
            treeWidget->setColumnCount(3);
            QStringList headers;
            headers<<"Name"<<"Type"<<"Value";
            treeWidget->setHeaderLabels(headers);
            treeWidget->setSortingEnabled( false );
            treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);
            treeWidget->show();
            
            parent = new QTreeWidgetItem(treeWidget);
            parent->setText(0, object->metaObject()->className());
            parent->setExpanded(true);
        }
        
        int i ,n = object->metaObject()->propertyCount();
        QTreeWidgetItem * property = new QTreeWidgetItem( parent );
        property->setText( 0, "[Properties]" );
        for (i = 0; i < n ; ++i){
            child = new QTreeWidgetItem(property);
            QMetaProperty p = object -> metaObject()->property(i);
            child->setText(0, p.name());
            QString typeName, value;
            if (p.isEnumType()) {
                QMetaEnum e = p.enumerator();
                typeName = QString("[Enum]%1::%2").arg(e.scope(),e.name());
                value = e.valueToKeys(p.read(object).toInt());
            }
            else {
                typeName = p.typeName();
                value = p.read(object).toString();
            }
            
            child->setText(1, typeName);
            child->setText(2, value);
        }
        
        foreach(QObject * obj, object->children())
        {
            child = new QTreeWidgetItem( parent );
            child->setText( 0, obj->metaObject()->className() );
            dumpRecursive( obj , child );
        }
    }
    void dumpObjTree(QObject * obj)
    {
        dumpRecursive( obj, 0 );
    }
    
}
#endif /* _QDUMPOBJECTTREE_ */
